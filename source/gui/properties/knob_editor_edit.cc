#include <knob_editor.h>
#include <trim_panel.h>
#include <util.h>
#include <properties.h>
#include <tab_widget.h>

void knob_editor::push_knob_or_tab()
{
    // inserta el knob o tab en el primer panel, si es que hay uno
    QWidget *panel = static_cast<properties *>(_properties)->get_first_panel();
    if (!panel)
        return;

    if (current_knob_type == "tab")
    {
        add_tab(panel);
        return;
    }

    add_knob(panel);
}

void knob_editor::add_knob(QWidget *panel, int index)
{
    if (index == -2 || !panel)
        return;

    trim_panel *_panel = static_cast<trim_panel *>(panel);

    QString label = knob_name->text();
    if (label.isEmpty())
        label = current_knob_type;

    QString name = get_available_knob_name(panel);
    QString tips = knob_tips->toPlainText();

    float min = 0;
    float max = 1;
    if (!minimum_edit->text().isEmpty())
        min = minimum_edit->text().toDouble();
    if (!maximum_edit->text().isEmpty())
        max = maximum_edit->text().toDouble();

    QString custom_tab = get_custom_tab_name(panel);

    QJsonObject knob_object;
    if (current_knob_type == "floating")
    {
        knob_object = {{"name", name},   {"type", "floating"},
                       {"label", label}, {"tooltip", tips},
                       {"minimum", min}, {"maximum", max},
                       {"default", min}, {"tab", custom_tab}};
    }
    else if (current_knob_type == "integer")
    {
        knob_object = {{"name", name},    {"type", "integer"}, {"label", label},
                       {"tooltip", tips}, {"minimum", min},    {"maximum", max},
                       {"default", min},  {"tab", custom_tab}};
    }
    else if (current_knob_type == "color")
    {
        knob_object = {{"name", name},
                       {"type", "color"},
                       {"label", label},
                       {"tooltip", tips},
                       {"minimum", min},
                       {"maximum", max},
                       {"centered_handler", true},
                       {"tab", custom_tab},
                       {"default", QJsonArray{0, 0, 0, 0}}};
    }
    else if (current_knob_type == "button")
    {
        knob_object = {{"name", name},
                       {"type", "button"},
                       {"label", label},
                       {"tab", custom_tab},
                       {"tooltip", tips}};
    }
    else if (current_knob_type == "choice")
    {
        knob_object = {{"name", name},
                       {"type", "choice"},
                       {"label", label},
                       {"tooltip", tips},
                       {"tab", custom_tab},
                       {"items", QJsonArray{}},
                       {"default", QJsonArray{0, ""}}};
    }
    else if (current_knob_type == "check_box")
    {
        knob_object = {{"name", name},       {"type", "check_box"},
                       {"label", label},     {"tooltip", tips},
                       {"over_line", false}, {"default", false},
                       {"tab", custom_tab}};
    }
    else if (current_knob_type == "text")
    {
        knob_object = {{"name", name},       {"type", "text"},
                       {"label", label},     {"tooltip", ""},
                       {"over_line", false}, {"default", tips},
                       {"tab", custom_tab}};
    }
    else if (current_knob_type == "file")
    {
        knob_object = {{"name", name},       {"type", "file"},
                       {"label", label},     {"tooltip", ""},
                       {"over_line", false}, {"default", tips},
                       {"tab", custom_tab}};
    }
    else if (current_knob_type == "position")
    {
        knob_object = {{"name", name},
                       {"type", "floating_dimensions"},
                       {"label", label},
                       {"tooltip", tips},
                       {"dimensions", 2},
                       {"over_line", false},
                       {"default", QJsonArray{0, 0}},
                       {"tab", custom_tab}};
    }
    else if (current_knob_type == "label")
    {
        knob_object = {{"name", name},
                       {"type", "label"},
                       {"label", label},
                       {"tab", custom_tab}};
    }
    else if (current_knob_type == "group")
    {
        knob_object = {{"name", name},   {"type", "group"},
                       {"label", label}, {"open", false},
                       {"knobs", 2},     {"tab", custom_tab}};
    }
    else if (current_knob_type == "separator")
    {
        knob_object = {
            {"name", name}, {"type", "separator"}, {"tab", custom_tab}};
    }

    if (knob_object.empty())
        return;

    QJsonArray &knobs = _panel->custom_knobs;

    if (index == -1)
        knobs.push_back(knob_object);
    else
    {
        if (knobs.count() < index)
            return;
        knobs.insert(index, knob_object);
    }

    _panel->update_custom_knobs();
}

QString knob_editor::add_tab(QWidget *panel, int index, QString preferred_name)
{
    trim_panel *_panel = static_cast<trim_panel *>(panel);
    if (!_panel)
        return "";

    QString tab_name = get_available_tab_name(panel, preferred_name);

    _panel->add_tab(tab_name, index);
    _panel->set_edit_mode(true);

    return tab_name;
}

QString knob_editor::get_available_tab_name(QWidget *panel, QString preferred_name) const
{
    tab_widget *_tab_widget =
        static_cast<trim_panel *>(panel)->get_tab_widget();

    QString name = knob_name->text();
    if (name.isEmpty())
        name = current_knob_type;

    if (!preferred_name.isEmpty())
        name = preferred_name;

    QStringList name_list;
    for (tab *_tab : _tab_widget->get_tabs())
        name_list.push_back(_tab->get_name());

    return get_available_name(name_list, name);
}

QString knob_editor::get_available_knob_name(QWidget *panel) const
{
    trim_panel *_panel = static_cast<trim_panel *>(panel);
    QJsonArray &knobs = _panel->custom_knobs;

    QString name = knob_name->text();
    if (name.isEmpty())
        name = current_knob_type;

    QStringList name_list;
    for (QJsonValue value : knobs)
        name_list.push_back(value.toObject().value("name").toString());

    name = name.replace(" ", "_").toLower();

    return get_available_name(name_list, name);
}

QString knob_editor::get_available_name(QStringList name_list, QString name) const
{
    auto get_basename = [](QString name) {
        QString str;
        for (QChar letter : name)
            if (!letter.isDigit())
                str += letter;
        return str;
    };

    QString basename = get_basename(name);
    QString available_name = basename;

    int number = 1;
    while (true)
    {
        if (!name_list.contains(available_name))
            break;

        available_name = basename + QString::number(number);
        number++;
    }

    return available_name;
}

QString knob_editor::get_custom_tab_name(QWidget *panel)
{
    // busca el tab actual, y si el un tab que los knobs de base, crea un tab
    // nuevo para agregar custom knobs

    trim_panel *_panel = static_cast<trim_panel *>(panel);
    tab_widget *_tab_widget = _panel->get_tab_widget();

    QString tab_name = _tab_widget->get_current_tab();
    QStringList only_read_tabs = _panel->get_only_read_tabs();

    if (!only_read_tabs.contains(tab_name))
        return tab_name;

    // busca un tab 'custom' que exista
    for (tab *_tab : _tab_widget->get_tabs())
    {
        tab_name = _tab->get_name();
        if (!only_read_tabs.contains(tab_name))
        {
            _tab_widget->set_tab(tab_name);
            return tab_name;
        }
    }

    tab_name = add_tab(panel, -1, "Custom");

    return tab_name;
}

knob *knob_editor::get_knob_under_cursor() const
{
    int vertical_gap = -15;

    QWidget *_widget =
        qApp->widgetAt({QCursor::pos().x(), QCursor::pos().y() + vertical_gap});
    while (_widget)
    {
        knob *_knob = dynamic_cast<knob *>(_widget);
        if (_knob)
            return _knob;

        _widget = _widget->parentWidget();
    }

    return nullptr;
}

QWidget *knob_editor::get_panel_under_cursor() const
{
    QWidget *_widget = qApp->widgetAt(QCursor::pos());
    return get_panel_from_widget(_widget);
}

QWidget *knob_editor::get_panel_from_widget(QWidget *widget) const
{
    while (widget)
    {
        trim_panel *panel = dynamic_cast<trim_panel *>(widget);
        if (panel)
            return panel;

        widget = widget->parentWidget();
    }

    return nullptr;
}

QWidget *knob_editor::get_tab_widget_under_cursor() const
{
    QWidget *_widget = qApp->widgetAt(QCursor::pos());
    while (_widget)
    {
        if (_widget->objectName() == "tab_widget")
            return _widget;

        _widget = _widget->parentWidget();
    }

    return nullptr;

}

void knob_editor::insert_division_to_tabs(QPointF position)
{
    QWidget *__tab_widget = get_tab_widget_under_cursor();
    if (!__tab_widget)
        return;

    current_panel = get_panel_from_widget(__tab_widget);
    if (!current_panel)
    {
        temp_vertical_widget->hide();
        return;
    }

    tab_widget *_tab_widget = static_cast<tab_widget *>(__tab_widget);

    int index = 0;
    for (tab *_tab : _tab_widget->get_tabs())
    {
        float tab_center_x = _tab->pos().x() + (_tab->size().width() / 2);
        if (position.x() < tab_center_x)
            break;

        index++;
    }

    QHBoxLayout *tabs_layout = _tab_widget->get_tab_bar_layout();
    if (index >= tabs_layout->count())
        tabs_layout->addWidget(temp_vertical_widget);
    else
        tabs_layout->insertWidget(index + 1, temp_vertical_widget);

    temp_vertical_widget->show();

    insert_index = index;
}

void knob_editor::insert_division_to_knobs()
{
    knob *_knob = get_knob_under_cursor();
    QWidget *panel = get_panel_under_cursor();

    if (_knob)
    {
        current_panel = _knob->get_panel();
        insert_index = get_index_knob(current_panel, _knob->get_name()) + 1;
        QVBoxLayout *layout = get_controls_layout(current_panel);
        if (layout)
        {
            layout->insertWidget(insert_index + 1, temp_widget);
            temp_widget->show();
        }
    }

    if (!panel)
    {
        temp_widget->hide();
        current_panel = nullptr;
    }
}

int knob_editor::get_index_knob(QWidget *panel, QString knob_name) const
{
    if (!panel)
        return -2;

    trim_panel *_panel = static_cast<trim_panel *>(panel);
    QJsonArray &knobs = _panel->custom_knobs;

    int index = 0;
    for (QJsonValue value : knobs)
    {
        if (value.toObject().value("name").toString() == knob_name)
            return index;

        index++;
    }

    return index;
}

QVBoxLayout *knob_editor::get_controls_layout(QWidget *panel) const
{
    return nullptr;
    if (!panel)
        return nullptr;

    // QLayout *layout =
        // static_cast<trim_panel *>(panel)->get_controls_tab()->layout();

    // return static_cast<QVBoxLayout *>(layout);
}

void knob_editor::mousePressEvent(QMouseEvent *event)
{
    insert_index = -2;
    current_panel = nullptr;
    this->setCursor(Qt::ClosedHandCursor);
}

void knob_editor::mouseMoveEvent(QMouseEvent *event)
{
    if (current_knob_type == "tab")
        insert_division_to_tabs(event->pos());
    else
        insert_division_to_knobs();
}

void knob_editor::mouseReleaseEvent(QMouseEvent *event)
{
    temp_widget->hide();
    temp_vertical_widget->hide();

    temp_widget->setParent(0);
    temp_vertical_widget->setParent(0);

    this->setCursor(Qt::ArrowCursor);
    add_knob(current_panel, insert_index);
    add_tab(current_panel, insert_index);
}

