#include <trim_panel.hpp>

trim_panel::trim_panel(
    properties *__properties,
    QString _name,
    QString _icon_name,
    QJsonArray *_knobs)
{
    _properties = __properties;
    name = _name;
    icon_name = _icon_name;

    this->setObjectName("trim_panel");
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    setup_ui();

    setup_knobs(_knobs);
}

trim_panel::~trim_panel()
{
}

void trim_panel::setup_ui()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);

    QWidget *butttons = top_buttons_setup_ui();

    layout->addWidget(butttons);

    tabs = tabs_ui();
    tabs->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    layout->addWidget(tabs);
}

void trim_panel::setup_knobs(QJsonArray *knobs)
{
    // Obtiene el ancho maximo a partir de las 'label'
    // para usarlo en el espacio inicial de cada parametro.
    int init_space_width = 0;
    for (int i = 0; i < knobs->count(); i++)
    {
        QJsonObject knob = knobs->at(i).toObject();
        QString label = knob.value("label").toString();
        QLabel _label(label);
        int width = _label.fontMetrics().boundingRect(_label.text()).width();

        if (width > init_space_width)
            init_space_width = width;
    }
    //
    //

    for (int i = 0; i < knobs->count(); i++)
    {
        QJsonObject knob = knobs->at(i).toObject();
        QString type = knob.value("type").toString();
        QString label = knob.value("label").toString();

        if (type == "color")
        {
            QJsonArray _default = knob.value("default").toArray();
            knob_color *_knob_color = new knob_color(
                init_space_width,
                label,
                QColor(
                    _default.at(0).toInt(),
                    _default.at(1).toInt(),
                    _default.at(2).toInt()));
            controls_layout->addWidget(_knob_color);
        }

        else if (type == "check_box")
        {
            bool _default = knob.value("default").toBool();
            knob_check_box *_knob_check_box = new knob_check_box(
                init_space_width,
                label,
                _default);
            controls_layout->addWidget(_knob_check_box);
        }

        else if (type == "file")
        {
            knob_file *_knob_file = new knob_file();
            controls_layout->addWidget(_knob_file);
        }

        else if (type == "choice")
        {
            knob_choice *_knob_choice = new knob_choice();
            controls_layout->addWidget(_knob_choice);
        }

        else if (type == "text")
        {
            knob_text *_knob_text = new knob_text();
            controls_layout->addWidget(_knob_text);
        }

        else if (type == "label")
        {
            knob_label *_knob_label = new knob_label();
            controls_layout->addWidget(_knob_label);
        }

        else if (type == "button")
        {
            knob_button *_knob_button = new knob_button();
            controls_layout->addWidget(_knob_button);
        }

        else if (type == "group")
        {
            knob_group *_knob_group = new knob_group();
            controls_layout->addWidget(_knob_group);
        }

        else if (type == "integer")
        {
            knob_integer *_knob_integer = new knob_integer();
            controls_layout->addWidget(_knob_integer);
        }

        else if (type == "floating")
        {
            float _default = knob.value("default").toDouble();
            knob_floating *_knob_floating = new knob_floating(
                init_space_width,
                label,
                _default);
            controls_layout->addWidget(_knob_floating);
        }

        else if (type == "separator")
        {
            knob_separator *_knob_separator = new knob_separator();
            controls_layout->addWidget(_knob_separator);
        }

        else if (type == "position")
        {
            knob_position *_knob_position = new knob_position();
            controls_layout->addWidget(_knob_position);
        }
    }
}

QWidget *trim_panel::top_buttons_setup_ui()
{
    QWidget *widget = new QWidget(this);
    widget->setObjectName("butttons");
    widget->setMaximumHeight(30);

    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setMargin(5);
    //
    //

    int icon_size = 20;

    QPushButton *settings = new QPushButton(widget);
    qt::set_icon(settings, "settings_a", icon_size);
    layout->addWidget(settings);

    QPushButton *center_node = new QPushButton(widget);
    qt::set_icon(center_node, "center_a", icon_size);
    layout->addWidget(center_node);

    layout->addStretch();

    QPushButton *icon_node = new QPushButton(widget);
    qt::set_icon(icon_node, icon_name, icon_size);
    layout->addWidget(icon_node);

    name_edit = new QLineEdit(widget);
    name_edit->setText(name);
    layout->addWidget(name_edit);

    layout->addStretch();

    // Minimize
    QPushButton *minimize = new QPushButton(widget);
    connect(minimize, &QPushButton::clicked, this, [=]() {
        this->maximize(!is_maximize);
    });
    qt::set_icon(minimize, "minimize_a", icon_size);
    layout->addWidget(minimize);
    // Minimize
    //

    QPushButton *maximize_button = new QPushButton(widget);
    qt::set_icon(maximize_button, "maximize_a", icon_size);
    layout->addWidget(maximize_button);

    QPushButton *close = new QPushButton(widget);
    connect(close, &QPushButton::clicked, this, [this]() {
        _properties->close_trim_panel(this->get_name());
    });
    qt::set_icon(close, "close_a", icon_size);
    layout->addWidget(close);

    return widget;
}

QTabWidget *trim_panel::tabs_ui()
{
    QTabWidget *tabs = new QTabWidget();

    QWidget *controls_tab = new QWidget();
    controls_layout = new QVBoxLayout(controls_tab);
    controls_tab->setObjectName("controls");

    QWidget *node = new QWidget();

    tabs->addTab(controls_tab, "Controls");
    tabs->addTab(node, "Node");

    return tabs;
}

QString trim_panel::get_name()
{
    return name;
}

void trim_panel::set_name(QString _name)
{
    name = _name;
    name_edit->setText(name);
}

void trim_panel::maximize(bool _maximize)
{
    _properties->hide();
    tabs->setVisible(_maximize);
    _properties->show();

    is_maximize = _maximize;
}