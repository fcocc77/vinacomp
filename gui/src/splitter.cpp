#include <splitter.hpp>

splitter::splitter()
{

    this->setObjectName("splitter");

    QWidget *top_buttons = new QWidget();
    setup_top_buttons(top_buttons);

    tab_section = new QWidget();

    tab_section->setObjectName("tab_section");

    QVBoxLayout *layout = new QVBoxLayout();

    layout->addWidget(top_buttons);
    layout->addWidget(tab_section);

    this->setLayout(layout);
}

splitter::~splitter()
{
}

void splitter::setup_top_buttons(QWidget *top_buttons)
{

    top_buttons->setObjectName("top_buttons");
    top_buttons->setMaximumHeight(70);

    QPushButton *menu_button = new QPushButton();
    menu_button->setMaximumWidth(100);

    qt::set_icon(menu_button, "resources/images/layout.png");

    // Menu
    QAction *split_vertical = new QAction("split_vertical");
    connect(split_vertical, &QAction::triggered, this, [this]() {
        split(Qt::Vertical);
    });
    split_vertical->setIcon(QIcon("resources/images/split_vertically.png"));

    QAction *split_horizontal = new QAction("split_horizontal");
    connect(split_horizontal, &QAction::triggered, this, [this]() {
        split(Qt::Horizontal);
    });
    split_horizontal->setIcon(QIcon("resources/images/split_horizontally.png"));

    // Cerrar panel
    QAction *close_panel_action = new QAction("Close Panel");
    connect(close_panel_action, &QAction::triggered, this, [this]() {
        close_panel();
    });
    close_panel_action->setIcon(QIcon("resources/images/close.png"));
    //

    QMenu *menu = new QMenu(this);

    menu->addAction(split_vertical);
    menu->addAction(split_horizontal);
    menu->addSeparator();
    menu->addAction(close_panel_action);
    //
    //

    connect(menu_button, &QPushButton::clicked, this, [=]() {
        menu->popup(QCursor::pos());
        menu->show();
    });

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(menu_button);
    layout->addStretch();

    top_buttons->setLayout(layout);
}

void splitter::split(Qt::Orientation orientation)
{

    auto parent = this->parentWidget();
    QLayout *layout = parent->layout();

    QSplitter *qsplitter = new QSplitter();
    qsplitter->setOrientation(orientation);

    splitter *new_splitter = new splitter();

    QWidget *container_a = new QWidget();
    container_a->setObjectName("container_a");
    QWidget *container_b = new QWidget();
    container_b->setObjectName("container_b");

    qt::add_widget(container_a, this);
    qt::add_widget(container_b, new_splitter);

    container_a->setParent(qsplitter);
    container_b->setParent(qsplitter);

    qsplitter->addWidget(container_a);
    qsplitter->addWidget(container_b);

    layout->addWidget(qsplitter);
}

void splitter::close_panel()
{
    QWidget *container = this->parentWidget();

    // si el container es el 'panels_layout' significa que es el
    // ultimo widget, y no se puede eliminar.
    if (container->objectName() == "panels_layout")
        return;
    //

    QWidget *qsplitter = container->parentWidget();
    QWidget *parent = qsplitter->parentWidget();

    QWidget *keep_widget;
    QWidget *delete_widget;

    if (container->objectName() == "container_a")
    {
        keep_widget = qsplitter->findChild<QWidget *>("container_b");
        delete_widget = qsplitter->findChild<QWidget *>("container_a");
    }
    else
    {
        keep_widget = qsplitter->findChild<QWidget *>("container_a");
        delete_widget = qsplitter->findChild<QWidget *>("container_b");
    }

    splitter *_splitter = keep_widget->findChild<splitter *>();
    parent->layout()->addWidget(_splitter);

    // borra los widgets sin usar
    delete_widget->setParent(0);
    delete_widget->deleteLater();

    parent->layout()->removeWidget(qsplitter);
    qsplitter->setParent(0);
    qsplitter->deleteLater();
}