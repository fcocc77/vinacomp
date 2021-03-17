#ifndef MERGE_NODE_H
#define MERGE_NODE_H

#include <QPainter>
#include <node_engine.h>

class merge_node : public node_engine
{
public:
    merge_node();
    ~merge_node();

    void render( render_data *rdata, QJsonObject *params ) override;
};

#endif // MERGE_NODE_H
