#ifndef KEYER_NODE_H
#define KEYER_NODE_H

#include <QPainter>
#include <node_engine.h>

class keyer_node : public node_engine
{
public:
    keyer_node();
    ~keyer_node();

    void render(render_data *rdata, QJsonObject *params) override;
};

#endif // KEYER_NODE_H
