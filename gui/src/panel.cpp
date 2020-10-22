#include <panel.hpp>

panel::panel(QList<QSplitter *> *_splitters, node_graph *__node_graph, viewer *__viewer)
{

    _node_graph = __node_graph;
    _viewer = __viewer;

    this->setObjectName("panel");

    splitters = _splitters;

    QWidget *top_buttons = new QWidget();
    setup_top_buttons(top_buttons);

    tab_section = new QTabWidget();

    tab_section->setObjectName("tab_section");

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(top_buttons);
    layout->addWidget(tab_section);

    this->setLayout(layout);
}

panel::~panel()
{
}

void panel::setup_top_buttons(QWidget *top_buttons)
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

    // Add NodeGraph
    QAction *add_node_graph_action = new QAction("Node Graph");
    connect(add_node_graph_action, &QAction::triggered, this, [this]() {
        int index = tab_section->addTab(_node_graph, "Node Graph");
        tab_section->setCurrentIndex(index);
    });
    //

    // Add Viewer
    QAction *add_viewer_action = new QAction("Viewer");
    connect(add_viewer_action, &QAction::triggered, this, [this]() {
        int index = tab_section->addTab(_viewer, "Viewer");
        tab_section->setCurrentIndex(index);
    });
    //

    QMenu *menu = new QMenu(this);

    menu->addAction(split_vertical);
    menu->addAction(split_horizontal);
    menu->addSeparator();
    menu->addAction(add_viewer_action);
    menu->addAction(add_node_graph_action);
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

QSplitter *panel::get_splitter()
{
    QWidget *container = this->parentWidget();
    QWidget *qsplitter = container->parentWidget();

    QSplitter *splitter;
    for (QSplitter *_splitter : *splitters)
    {
        if (_splitter->objectName() == qsplitter->objectName())
        {
            splitter = _splitter;
            break;
        }
    }

    return splitter;
}

panel *panel::split(Qt::Orientation orientation)
{

    auto parent = this->parentWidget();
    QLayout *layout = parent->layout();

    QSplitter *qsplitter = new QSplitter();

    if (parent->objectName() == "panels_layout")
        qsplitter->setObjectName("splitter");
    else
        qsplitter->setObjectName("splitter" + util::hash());

    splitters->push_back(qsplitter);

    qsplitter->setOrientation(orientation);

    panel *new_panel = new panel(splitters, _node_graph, _viewer);

    QWidget *container_a = new QWidget();
    QWidget *container_b = new QWidget();
    container_a->setObjectName("container_a");
    container_b->setObjectName("container_b");

    qt::add_widget(container_a, this);
    qt::add_widget(container_b, new_panel);

    container_a->setParent(qsplitter);
    container_b->setParent(qsplitter);

    qsplitter->addWidget(container_a);
    qsplitter->addWidget(container_b);

    layout->addWidget(qsplitter);

    return new_panel;
}

void panel::close_panel()
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

    panel *keep_panel = keep_widget->findChild<panel *>();
    parent->layout()->addWidget(keep_panel);

    // borra los widgets sin usar
    delete_widget->setParent(0);
    delete_widget->deleteLater();

    // Borra el 'qsplitter' de la lista de 'splitters'
    int i = 0;
    for (QSplitter *splitter : *splitters)
    {
        if (splitter->objectName() == qsplitter->objectName())
            break;
        i++;
    }
    splitters->removeAt(i);
    //
    //

    parent->layout()->removeWidget(qsplitter);
    qsplitter->setParent(0);
    qsplitter->deleteLater();
}