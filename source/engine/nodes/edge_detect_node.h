#ifndef EDGE_DETECT_NODE_H
#define EDGE_DETECT_NODE_H

#include <QPainter>
#include <node_engine.h>

class edge_detect_node : public node_engine
{
public:
    edge_detect_node();
    ~edge_detect_node();

    void render( render_data *rdata, QJsonObject *params ) override;
};

#endif // EDGE_DETECT_NODE_H
