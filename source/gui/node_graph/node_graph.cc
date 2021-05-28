#include <node_graph.h>
#include <trim_panel.h>
#include <vinacomp.h>

node_graph::node_graph(QWidget *__vinacomp, project_struct *_project,
                       properties *_properties, QString _group_name,
                       node_group *__node_group)

    : project(_project)
    , _node_group(__node_group)
    , group_name(_group_name)
    , _vinacomp(__vinacomp)
{

    this->setObjectName("node_graph");
    layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    nodes_loaded = new nodes_load(this);

    _node_view = new node_view(_vinacomp, _properties, nodes_loaded, project, this);
    _maker = new maker(_vinacomp, _properties, nodes_loaded, _node_view, this);

    init_menu();

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
}

node_graph::~node_graph()
{
    delete layout;
    delete nodes_loaded;
    delete _node_view;
    delete _nodes_bar;
    delete _maker;
}

void node_graph::save_nodes_attributes_to_project()
{
    // ya que algunos atributos no es importante para el renderizado,
    // estos atributos del proyecto no se actualiza, dinamicamente
    // asi que solo se actualiza cuando se guarda el proyecto.

    QList<node *> all_nodes;

    vinacomp *vina = static_cast<vinacomp *>(_vinacomp);
    for (node_graph *group_graph : *vina->get_groups_node_graph())
        for (node *_node : *group_graph->get_node_view()->get_nodes())
            all_nodes.push_back(_node);

    for (node *_node : *_node_view->get_nodes())
        all_nodes.push_back(_node);

    for (node *_node : all_nodes)
    {
        node_struct &__node = project->nodes[_node->get_name()];
        trim_panel *panel = _node->get_trim_panel();

        __node.pos = {_node->x(), _node->y()};
        __node.color = _node->get_color();
        __node.size = _node->get_size();
        __node.z_value = _node->zValue();
        __node.handler_node = _node->get_handler_node_name();

        if (panel)
        {
            node_gui *_node_gui = panel->get_node_gui();
            if (_node_gui)
                __node.script = _node_gui->get_script();
        }
    }
}

void node_graph::restore_tree()
{
    auto nodes = project->get_nodes_from_group(get_group_name());

    // crea los nodos
    for (node_struct node : nodes)
        _node_view->create_node(node, true, true);
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

            input_wire *input = _node_view->get_input_wire(_node, index);
            if (input)
                input->connect_node(node_to_connect, false);
        }

        _node->add_handler_node(node_data.handler_node);
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
    _node_view->freeze_current_rect();
}

void node_graph::clear_tree()
{
    auto nodes = *_node_view->get_nodes();

    for (node *_node : nodes)
        _node_view->delete_node(_node);
}
