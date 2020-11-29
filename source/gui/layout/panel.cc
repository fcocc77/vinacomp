#include <panel.h>
#include <QToolButton>

panel::panel(QWidget *_panels_layout,
             QList<QSplitter *> *_splitters,
             node_graph *__node_graph,
             viewer *__viewer,
             script_editor *__script_editor,
             properties *__properties,
             curve_editor *__curve_editor)

    : panels_layout(_panels_layout),
      _node_graph(__node_graph),
      _viewer(__viewer),
      _script_editor(__script_editor),
      _properties(__properties),
      _curve_editor(__curve_editor),
      splitters(_splitters)

{
    this->setObjectName("panel");

    tab_section = new tab_widget();
    QPushButton *cornel_button = setup_cornel_buttons();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 4, 0, 0);
    layout->addWidget(tab_section);

    this->setLayout(layout);
}

panel::~panel()
{
}

QPushButton *panel::setup_cornel_buttons()
{
    QPushButton *menu_button = tab_section->add_cornel_button("layout_a");

    // Menu
    QAction *split_vertical = new QAction("Vertical Split");
    connect(split_vertical, &QAction::triggered, this, [this]() {
        split(Qt::Horizontal);
    });
    split_vertical->setIcon(QIcon("resources/images/vertical_split_a.png"));

    QAction *split_horizontal = new QAction("Horizontal Split");
    connect(split_horizontal, &QAction::triggered, this, [this]() {
        split(Qt::Vertical);
    });
    split_horizontal->setIcon(QIcon("resources/images/horizontal_split_a.png"));

    // Cerrar panel
    QAction *close_panel_action = new QAction("Close Panel");
    connect(close_panel_action, &QAction::triggered, this, [this]() {
        close_panel();
    });
    close_panel_action->setIcon(QIcon("resources/images/close_a.png"));
    //

    // Add NodeGraph
    QAction *add_node_graph_action = new QAction("Node Graph");
    connect(add_node_graph_action, &QAction::triggered, this, [this]() {
        add_tab("node_graph");
    });
    add_node_graph_action->setIcon(QIcon("resources/images/node_graph_a.png"));
    //

    // Add Viewer
    QAction *add_viewer_action = new QAction("Viewer");
    connect(add_viewer_action, &QAction::triggered, this, [this]() {
        add_tab("viewer");
    });
    add_viewer_action->setIcon(QIcon("resources/images/viewer_a.png"));
    //

    // Add Script Editor
    QAction *add_script_editor_action = new QAction("Script Editor");
    connect(add_script_editor_action, &QAction::triggered, this, [this]() {
        add_tab("script_editor");
    });
    add_script_editor_action->setIcon(QIcon("resources/images/script_editor_a.png"));
    //

    // Add Curve Editor
    QAction *add_curve_editor_action = new QAction("Curve Editor");
    connect(add_curve_editor_action, &QAction::triggered, this, [this]() {
        add_tab("curve_editor");
    });
    add_curve_editor_action->setIcon(QIcon("resources/images/curve_editor_a.png"));
    //

    // Add Properties
    QAction *add_properties_action = new QAction("Properties");
    connect(add_properties_action, &QAction::triggered, this, [this]() {
        add_tab("properties");
    });
    add_properties_action->setIcon(QIcon("resources/images/properties_a.png"));
    //

    QMenu *menu = new QMenu(this);

    menu->addAction(split_vertical);
    menu->addAction(split_horizontal);
    menu->addSeparator();
    menu->addAction(add_viewer_action);
    menu->addAction(add_node_graph_action);
    menu->addAction(add_curve_editor_action);
    menu->addAction(add_script_editor_action);
    menu->addAction(add_properties_action);
    menu->addSeparator();
    menu->addAction(close_panel_action);
    //
    //

    connect(menu_button, &QPushButton::clicked, this, [=]() {
        menu->exec(menu_button->mapToGlobal({0, menu_button->height()}));
    });

    return menu_button;
}

void panel::add_tabs(QStringList tabs_list)
{
    for (QString tab_name : tabs_list)
        add_tab(tab_name);
}

void panel::remove_all_tab()
{
    tab_section->clear();
    tabs_list.clear();
}

QString panel::get_tab_label(QString name)
{
    if (name == "node_graph")
        return "Node Graph";

    else if (name == "viewer")
        return "Viewer";

    else if (name == "curve_editor")
        return "Curve Editor";

    else if (name == "script_editor")
        return "Script Editor";

    else if (name == "properties")
        return "Properties";

    else
        return "";
}

void panel::set_index(int index)
{
    tab_section->set_index(index);
}

void panel::add_tab(QString name)
{
    QWidget *tab;
    if (name == "node_graph")
        tab = _node_graph;

    else if (name == "viewer")
        tab = _viewer;

    else if (name == "curve_editor")
        tab = _curve_editor;

    else if (name == "script_editor")
        tab = _script_editor;

    else if (name == "properties")
        tab = _properties;

    else
        return;

    tab_section->add_tab(tab, get_tab_label(name));

    // el tab que se va a agregar en este panel se borra en
    // todos los paneles, si es que esta en alguno.
    QList<panel *> panels = panels_layout->findChildren<panel *>("panel");

    for (panel *_panel : panels)
        _panel->tabs_list.removeOne(name);

    //
    //

    tabs_list.push_back(name);
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

    panel *new_panel = new panel(panels_layout, splitters, _node_graph, _viewer, _script_editor, _properties, _curve_editor);

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

    qsplitter->setSizes({100, 100});

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

    remove_all_tab();

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

    delete tab_section;

    parent->layout()->removeWidget(qsplitter);
    qsplitter->setParent(0);
    qsplitter->deleteLater();
}