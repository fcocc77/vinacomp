#ifndef CROP_NODE_H
#define CROP_NODE_H

#include <QPainter>
#include <node_engine.h>

class crop_node : public node_engine
{
public:
    crop_node();
    ~crop_node();

    void render(render_data *rdata, QJsonObject *params) override;
};

#endif // CROP_NODE_H
