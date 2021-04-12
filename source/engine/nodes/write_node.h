#ifndef WRITE_NODE_H
#define WRITE_NODE_H

#include <node_engine.h>

class write_node : public node_engine
{
public:
    write_node();
    ~write_node();

    void render(render_data *rdata, QJsonObject *params) override;
};

#endif // WRITE_NODE_H
