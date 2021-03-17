#ifndef DISSOLVE_NODE_H
#define DISSOLVE_NODE_H

#include <QPainter>
#include <node_engine.h>

class dissolve_node : public node_engine
{
public:
    dissolve_node();
    ~dissolve_node();

    void render(render_data *rdata, QJsonObject *params) override;
};

#endif // DISSOLVE_NODE_H
