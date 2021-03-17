#ifndef BBOX_ADJUST_NODE_H
#define BBOX_ADJUST_NODE_H

#include <QPainter>
#include <node_engine.h>

class bbox_adjust_node : public node_engine
{
public:
    bbox_adjust_node();
    ~bbox_adjust_node();

    void render(render_data *rdata, QJsonObject *params) override;
};

#endif // BBOX_ADJUST_NODE_H
