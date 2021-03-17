#ifndef SWITCH_NODE_H
#define SWITCH_NODE_H

#include <QPainter>
#include <node_engine.h>

class switch_node : public node_engine
{
public:
    switch_node();
    ~switch_node();

    void render( render_data *rdata, QJsonObject *params ) override;
};

#endif // SWITCH_NODE_H
