#include "node_group.h"
#include <node_graph.h>
#include <vinacomp.h>

node_group::node_group(node_props _props,
                       QMap<QString, node *> *_selected_nodes,
                       QWidget *_node_graph)

    : node_rect(_props, _selected_nodes, _node_graph)
    , props(_props)
    , group_node_graph(nullptr)
{

    // temporalmente
    open_group();
}

node_group::~node_group()
{
    if (group_node_graph)
        delete group_node_graph;
}

void node_group::open_group()
{
    // crea el node_graph de grupo solo cuando se necesite abrir el grupo, para
    // economizar memoria
    if (!group_node_graph)
        group_node_graph =
            new node_graph(props.vinacomp, props.project, props._properties);

    node_graph *graph = static_cast<node_graph *>(group_node_graph);

    static_cast<vinacomp *>(props.vinacomp)
        ->get_panels_layout()
        ->add_node_graph_group(graph, get_name());
}
