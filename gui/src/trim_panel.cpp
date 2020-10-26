#include <trim_panel.hpp>

trim_panel::trim_panel(/* args */)
{

    setup_ui();
}

trim_panel::~trim_panel()
{
}

void trim_panel::setup_ui()
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);

    QWidget *butttons = top_buttons_setup_ui();

    layout->addWidget(butttons);

    QWidget *knobs = new QWidget();
    QVBoxLayout *knobs_layout = new QVBoxLayout();
    knobs->setLayout(knobs_layout);
    layout->addWidget(knobs);
    //
    //
}

QWidget *trim_panel::top_buttons_setup_ui()
{
    QWidget *widget = new QWidget();
    widget->setObjectName("butttons");
    widget->setMaximumHeight(50);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);

    widget->setLayout(layout);
    //
    //

    int icon_size = 20;

    QPushButton *settings = new QPushButton();
    qt::set_icon(settings, "settings_a", icon_size);
    layout->addWidget(settings);

    QPushButton *center_node = new QPushButton();
    qt::set_icon(center_node, "center_a", icon_size);
    layout->addWidget(center_node);

    layout->addStretch();

    QPushButton *icon_node = new QPushButton();
    qt::set_icon(icon_node, "transform_a", icon_size);
    layout->addWidget(icon_node);

    QLineEdit *node_name = new QLineEdit();
    layout->addWidget(node_name);

    layout->addStretch();

    QPushButton *minimize = new QPushButton();
    qt::set_icon(minimize, "minimize_a", icon_size);
    layout->addWidget(minimize);

    QPushButton *maximize = new QPushButton();
    qt::set_icon(maximize, "maximize_a", icon_size);
    layout->addWidget(maximize);

    QPushButton *close = new QPushButton();
    qt::set_icon(close, "close_a", icon_size);
    layout->addWidget(close);

    return widget;
}
