#include <node_graph.h>
#include <trim_panel.h>

node_graph::node_graph(QWidget *_vinacomp, project_struct *_project,
                       properties *_properties, QString _group_name,
                       node_group *__node_group)

    : project(_project)
    , _node_group(__node_group)
    , group_name(_group_name)
{

    this->setObjectName("node_graph");
    layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    nodes_loaded = new nodes_load();

    _node_view = new node_view(_vinacomp, _properties, nodes_loaded, this);
    _maker = new maker(_vinacomp, _properties, nodes_loaded, _node_view, this);
    _nodes_bar = new nodes_bar(this, _maker, nodes_loaded);

    connect(_node_view, &node_view::clicked, this,
            [=]() { _maker->get_finder()->hide(); });

    connect(_nodes_bar, &nodes_bar::search_changed, _node_view,
            &node_view::find_nodes);

    // shortcut de busqueda de nodos
    qt::shortcut("Ctrl+F", this,
                 [this]() { _nodes_bar->focus_to_find_edit(); });
    qt::shortcut("Ctrl+P", this,
                 [this]() { _nodes_bar->focus_to_find_edit(); });
    //

    layout->addWidget(_nodes_bar);
    layout->addWidget(_node_view);

    init_menu();
}

node_graph::~node_graph() {}

void node_graph::update_node_position_from_project()
{
    // ya que la posicion no es importante para el renderizado,
    // la posicion de los nodos del proyecto no se actualiza, dinamicamente
    // asi que solo se actualiza cuando se guarda el proyecto.
    for (node *_node : *_node_view->get_nodes())
        project->nodes[_node->get_name()].pos = {_node->x(), _node->y()};
}

QList<node_struct> node_graph::get_nodes_from_group() const
{
    QString group_name = get_group_name();

    if (group_name.isEmpty())
    {
        // crea lista con los nodos principales ya que no son grupos al no tener
        // punto
        QList<node_struct> main_nodes;
        for (QString name : project->nodes.keys())
        {
            auto node = project->nodes.value(name);
            if (!name.contains('.'))
                main_nodes.push_back(node);
        }

        return main_nodes;
    }

    // obtiene todos los nodos que pertenecen a un grupo especifico
    group_name += ".";
    QList<node_struct> nodes_from_group;
    for (QString name : project->nodes.keys())
    {
        QString group_of_node = name.left(name.lastIndexOf('.') + 1);
        if (group_of_node == group_name)
            nodes_from_group.push_back(project->nodes.value(name));
    }

    return nodes_from_group;
}

void node_graph::restore_tree()
{
    auto nodes = get_nodes_from_group();

    // crea los nodos
    for (node_struct node : nodes)
        _node_view->create_node(node.name, node.color, node.type, node.pos);
    //

    // conecta todos los nodos
    for (node_struct node_data : nodes)
    {
        node *_node = _node_view->get_nodes()->value(node_data.name);

        for (QString index_name : node_data.inputs.keys())
        {
            QString connected_node =
                node_data.inputs.value(index_name).toString();
            node *node_to_connect = _node_view->get_node(connected_node);

            int index = index_name.replace("in", "").toInt();

            node_link *link = _node_view->get_node_link(_node, index);
            if (link)
                link->connect_node(node_to_connect, false);
        }
    }
}

void node_graph::save_scene_data()
{
    // guarda el rectangulo del viewport
    project->global.node_view_rect = _node_view->get_last_rect();
}

void node_graph::restore_scene_data()
{
    _node_view->restore_rect(project->global.node_view_rect);
}
