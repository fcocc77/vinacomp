#include "node_plugin.h"

node_plugin::node_plugin(node_props _props,
                         QMap<QString, node *> *_selected_nodes,
                         QWidget *_node_graph)

    : node_rect(_props, _selected_nodes, _node_graph)
    , props(_props)
{
}

node_plugin::~node_plugin() {}

void node_plugin::set_name(QString name)
{
    props.project->replace_parent_name_to_children(get_name(), name);
    node_rect::set_name(name);
}
