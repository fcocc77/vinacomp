#ifndef ERODE_NODE_H
#define ERODE_NODE_H

#include <QPainter>
#include <node_engine.h>

class erode_node : public node_engine
{
public:
    erode_node();
    ~erode_node();

    void render( render_data *rdata, QJsonObject *params ) override;
};

#endif // ERODE_NODE_H
