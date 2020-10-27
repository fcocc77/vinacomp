#include <properties.hpp>

properties::properties(/* args */)
{
    this->setObjectName("properties");
    setup_ui();
}

properties::~properties()
{
}

void properties::setup_ui()
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);
    this->setLayout(layout);

    QWidget *butttons = top_buttons_setup_ui();

    layout->addWidget(butttons);

    QWidget *trim_panels = new QWidget();
    QVBoxLayout *trim_panels_layout = new QVBoxLayout();
    trim_panels_layout->setContentsMargins(0, 0, 0, 0);
    trim_panels->setLayout(trim_panels_layout);
    layout->addWidget(trim_panels);
    layout->addStretch();
    //
    //

    trim_panel *_trim_panel = new trim_panel();
    trim_panels_layout->addWidget(_trim_panel);
}

QWidget *properties::top_buttons_setup_ui()
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

    QLineEdit *max_panels = new QLineEdit();
    max_panels->setMaximumWidth(50);
    layout->addWidget(max_panels);

    QPushButton *clear = new QPushButton();
    qt::set_icon(clear, "close_a", icon_size);
    layout->addWidget(clear);

    QPushButton *lock = new QPushButton();
    qt::set_icon(lock, "lock_a", icon_size);
    layout->addWidget(lock);

    QPushButton *maximize_all = new QPushButton();
    qt::set_icon(maximize_all, "maximize_a", icon_size);
    layout->addWidget(maximize_all);

    layout->addStretch();

    return widget;
}
