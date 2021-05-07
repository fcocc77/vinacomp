#include <properties.h>
#include <trim_panel.h>
#include <vinacomp.h>
#include <knob.h>
#include <button.h>

properties::properties(QWidget *__vinacomp)
    : _vinacomp(__vinacomp)
    , is_maximize(true)
    , max_panels(10)
    , edit_mode(false)
{
    this->setObjectName("properties");
    setup_ui();
}

properties::~properties() {}

void properties::setup_ui()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);

    QWidget *butttons = top_buttons_setup_ui();
    layout->addWidget(butttons);

    _knob_editor = new knob_editor(this);
    _knob_editor->hide();
    layout->addWidget(_knob_editor);

    trim_panels = new QWidget(this);
    trim_panels->setObjectName("trim_panels");
    trim_panels_layout = new QVBoxLayout(trim_panels);
    trim_panels_layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    trim_panels_layout->setContentsMargins(0, 0, 0, 0);
    trim_panels_layout->setDirection(QBoxLayout::BottomToTop);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(trim_panels);

    layout->addWidget(scrollArea);
}

QWidget *properties::top_buttons_setup_ui()
{
    QWidget *widget = new QWidget();
    widget->setObjectName("butttons");
    widget->setMaximumHeight(50);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(10, 5, 0, 0);

    widget->setLayout(layout);
    //
    //

    int icon_size = 20;

    QLineEdit *max_panels_edit = new QLineEdit();
    max_panels_edit->setText(QString::number(max_panels));
    connect(max_panels_edit, &QLineEdit::editingFinished, this, [=]() {
        int max = max_panels_edit->text().toInt();
        if (max < 1)
            max_panels_edit->setText(QString::number(max_panels));
        else
            max_panels = max;
        limit_panels(max_panels);
    });

    max_panels_edit->setMaximumWidth(50);
    layout->addWidget(max_panels_edit);

    QPushButton *clear = new QPushButton();
    connect(clear, &QPushButton::clicked, this, &properties::close_all);
    qt::set_icon(clear, "close_normal", icon_size);
    layout->addWidget(clear);

    button *edit_knobs = new button();
    edit_knobs->set_checkable();
    edit_knobs->set_icon("edit", icon_size);
    connect(edit_knobs, &button::clicked, this, [=](bool checked) {
        edit_mode = checked;
        _knob_editor->setVisible(edit_mode);
        set_edit_mode(edit_mode);
    });
    layout->addWidget(edit_knobs);

    QPushButton *maximize_all = new QPushButton();
    connect(maximize_all, &QPushButton::clicked, this, &properties::minimize_all_panels);
    qt::set_icon(maximize_all, "maximize_normal", icon_size);
    layout->addWidget(maximize_all);

    layout->addStretch();

    return widget;
}

void properties::set_edit_mode(bool enable)
{
    for (QWidget *panel : get_trim_panels())
        static_cast<trim_panel *>(panel)->set_edit_mode(enable);
}

void properties::minimize_all_panels()
{
    is_maximize = !is_maximize;
    for (QWidget *widget : get_trim_panels())
    {
        trim_panel *_trim_panel = static_cast<trim_panel *>(widget);
        _trim_panel->maximize(is_maximize);
    }
}

void properties::close_all()
{
    for (QWidget *widget : get_trim_panels())
    {
        trim_panel *_trim_panel = static_cast<trim_panel *>(widget);
        close_trim_panel(_trim_panel->get_name());
    }
}

void properties::close_trim_panel(QString panel_name)
{
    QWidget *panel = get_trim_panel(panel_name);
    trim_panel *_trim_panel = static_cast<trim_panel *>(panel);

    if (!_trim_panel)
        return;

    _trim_panel->leave_properties();

    auto _curve_editor = static_cast<vinacomp *>(_vinacomp)->get_curve_editor();
    _curve_editor->delete_panel(_trim_panel);

    update_viewers_handlers();
}

QWidget *properties::get_trim_panel(QString panel_name)
{
    // encuentra un 'trim_panel' en el layout, a partir de un nombre,
    // si no esta devuelve un NULL.
    for (QWidget *widget : get_trim_panels())
    {
        trim_panel *_trim_panel = static_cast<trim_panel *>(widget);
        if (_trim_panel)
            if (_trim_panel->get_name() == panel_name)
                return widget;
    }

    return nullptr;
}

QWidget *properties::get_trim_panel(int index)
{
    QLayoutItem *item = trim_panels_layout->itemAt(index);
    if (!item)
        return nullptr;

    QWidget *widget = item->widget();
    trim_panel *_trim_panel = static_cast<trim_panel *>(widget);
    if (_trim_panel)
        return _trim_panel;

    return nullptr;
}

QWidget *properties::get_first_panel()
{
    return get_trim_panel(trim_panels_layout->count() - 1);
}

QList<QWidget *> properties::get_trim_panels()
{
    QList<QWidget *> panels;
    for (int i = 0; i < trim_panels_layout->count(); i++)
    {
        QWidget *_trim_panel = get_trim_panel(i);
        if (_trim_panel)
            panels.push_back(_trim_panel);
    }

    return panels;
}

void properties::limit_panels(int amount)
{
    // limita la cantidad de paneles que puede tener las propiedades
    int count = trim_panels_layout->count();

    while (count > amount + 1)
    {
        QLayoutItem *layout_item = trim_panels_layout->itemAt(1);
        if (layout_item)
        {
            QWidget *widget = layout_item->widget();
            trim_panel *first_panel = static_cast<trim_panel *>(widget);

            if (first_panel)
                close_trim_panel(first_panel->get_name());
        }

        count = trim_panels_layout->count();
    }
}

void properties::update_viewers_handlers()
{
    auto *viewers = static_cast<vinacomp *>(_vinacomp)->get_viewers();
    for (viewer *_viewer : *viewers)
        _viewer->get_viewer_gl()->handlers_update();
}

void properties::update_animated_knobs()
{
    // actualiza todos los 'knob' que estan animados o con expression
    // al frame correspondiente
    for (QWidget *panel : get_trim_panels())
    {
        trim_panel *_trim_panel = static_cast<trim_panel *>(panel);
        auto *knobs = _trim_panel->get_knobs();
        for (QString key : knobs->keys())
        {
            knob *_knob = knobs->value(key);
            if (_knob->is_animated())
                _knob->update_animated();
        }
    }
}

void properties::update_curve(QWidget *_trim_panel)
{
    auto _curve_editor = static_cast<vinacomp *>(_vinacomp)->get_curve_editor();
    trim_panel *panel = static_cast<trim_panel *>(_trim_panel);

    _curve_editor->update_from_trim_panel(panel);
}
