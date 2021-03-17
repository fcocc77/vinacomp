#ifndef INVERT_NODE_H
#define INVERT_NODE_H

#include <node_engine.h>

class invert_node : public node_engine
{
public:
    invert_node();
    ~invert_node();

    void render( render_data *rdata, QJsonObject *params ) override;
};

#endif // INVERT_NODE_H
