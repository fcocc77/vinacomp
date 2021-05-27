#ifndef NODE_PLUGIN_H
#define NODE_PLUGIN_H

#include "node_rect.h"

class node_plugin : public node_rect
{
private:
    node_props props;
    void set_name(QString name) override;

public:
    node_plugin(node_props _props, QMap<QString, node *> *_selected_nodes,
                QWidget *_node_graph);
    ~node_plugin();
};

#endif // NODE_PLUGIN_H
