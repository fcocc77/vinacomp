#ifndef POSITION_NODE_H
#define POSITION_NODE_H

#include <node_engine.h>

#include <opencv2/opencv.hpp>

class position_node : public node_engine
{
public:
    position_node();
    ~position_node();

    void render( render_data *rdata, QJsonObject *params ) override;
};

#endif // POSITION_NODE_H
