#ifndef VIEWER_NODE_H
#define VIEWER_NODE_H

#include <QImage>

#include <node_engine.h>

class viewer_node : public node_engine
{
public:
    viewer_node();
    ~viewer_node();

    void render( render_data *rdata, QJsonObject *params ) override;
};

#endif // VIEWER_NODE_H
