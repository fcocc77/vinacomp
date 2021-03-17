#ifndef REFORMAT_NODE_H
#define REFORMAT_NODE_H

#include <node_engine.h>

class reformat_node : public node_engine
{
public:
    reformat_node();
    ~reformat_node();

    void render(render_data *rdata, QJsonObject *params) override;
};

#endif // REFORMAT_NODE_H
