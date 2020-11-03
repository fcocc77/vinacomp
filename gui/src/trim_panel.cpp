#include <trim_panel.hpp>

trim_panel::trim_panel(
    properties *__properties,
    QString _name,
    QString _icon_name)
{
    _properties = __properties;
    name = _name;
    icon_name = _icon_name;

    this->setObjectName("trim_panel");
    setup_ui();
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
    layout->addWidget(tabs);
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

    QLineEdit *node_name = new QLineEdit(widget);
    node_name->setText(name);
    layout->addWidget(node_name);

    layout->addStretch();

    // Minimize
    QPushButton *minimize = new QPushButton(widget);
    connect(minimize, &QPushButton::clicked, this, [this]() {
        is_minimize = !is_minimize;
        _properties->hide();
        tabs->setVisible(!is_minimize);
        _properties->show();
    });
    qt::set_icon(minimize, "minimize_a", icon_size);
    layout->addWidget(minimize);
    // Minimize
    //

    QPushButton *maximize = new QPushButton(widget);
    qt::set_icon(maximize, "maximize_a", icon_size);
    layout->addWidget(maximize);

    QPushButton *close = new QPushButton(widget);
    connect(close, &QPushButton::clicked, this, [this]() {
        _properties->close_trim_panel(this);
    });
    qt::set_icon(close, "close_a", icon_size);
    layout->addWidget(close);

    return widget;
}

QTabWidget *trim_panel::tabs_ui()
{

    QTabWidget *tabs = new QTabWidget();

    QWidget *controls = new QWidget();
    QVBoxLayout *controls_layout = new QVBoxLayout(controls);
    controls->setObjectName("controls");

    QWidget *node = new QWidget();

    tabs->addTab(controls, "Controls");
    tabs->addTab(node, "Node");

    //
    knob_color *_knob_color = new knob_color();
    controls_layout->addWidget(_knob_color);

    knob_check_box *_knob_check_box = new knob_check_box();
    controls_layout->addWidget(_knob_check_box);

    knob_file *_knob_file = new knob_file();
    controls_layout->addWidget(_knob_file);

    knob_choice *_knob_choice = new knob_choice();
    controls_layout->addWidget(_knob_choice);

    knob_text *_knob_text = new knob_text();
    controls_layout->addWidget(_knob_text);

    knob_label *_knob_label = new knob_label();
    controls_layout->addWidget(_knob_label);

    knob_button *_knob_button = new knob_button();
    controls_layout->addWidget(_knob_button);

    knob_group *_knob_group = new knob_group();
    controls_layout->addWidget(_knob_group);

    knob_integer *_knob_integer = new knob_integer();
    controls_layout->addWidget(_knob_integer);

    knob_floating *_knob_floating = new knob_floating();
    controls_layout->addWidget(_knob_floating);

    knob_separator *_knob_separator = new knob_separator();
    controls_layout->addWidget(_knob_separator);

    knob_position *_knob_position = new knob_position();
    controls_layout->addWidget(_knob_position);

    controls_layout->addStretch();

    return tabs;
}

QString trim_panel::get_name()
{
    return name;
}