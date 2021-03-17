#ifndef MIRROR_NODE_H
#define MIRROR_NODE_H

#include <QPainter>
#include <node_engine.h>

void flip_flop(cv::Mat &image, bool flip = false, bool flop = false);

class mirror_node : public node_engine
{
public:
    mirror_node();
    ~mirror_node();

    void render(render_data *rdata, QJsonObject *params) override;
};

#endif // MIRROR_NODE_H
