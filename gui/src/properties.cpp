#include <properties.hpp>
#include <trim_panel.hpp>

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
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);

    QWidget *butttons = top_buttons_setup_ui();

    layout->addWidget(butttons);

    trim_panels = new QWidget(this);
    trim_panels_layout = new QVBoxLayout(trim_panels);
    trim_panels_layout->setContentsMargins(0, 0, 0, 0);
    trim_panels_layout->setDirection(QBoxLayout::BottomToTop);
    trim_panels_layout->addStretch();

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
