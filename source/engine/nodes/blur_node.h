#ifndef BLUR_NODE_H
#define BLUR_NODE_H

#include <QImage>

#include <node_engine.h>
#include <util.h>

class blur_node : public node_engine
{
public:
    blur_node();
    ~blur_node();

    void render(render_data *rdata, QJsonObject *params) override;
};

#endif // BLUR_NODE_H
