#ifndef ROTO_NODE_H
#define ROTO_NODE_H

#include <node_engine.h>

class roto_node : public node_engine
{
public:
    roto_node();
    ~roto_node();

    void render(render_data *rdata, QJsonObject *params) override;
};

#endif // ROTO_NODE_H
