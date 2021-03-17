
#ifndef CONSTANT_NODE_H
#define CONSTANT_NODE_H

#include <node_engine.h>

class constant_node : public node_engine
{
public:
    constant_node();
    ~constant_node();

    void render(render_data *rdata, QJsonObject *params) override;
};

#endif // CONSTANT_NODE_H
