#ifndef IDISTORT_NODE_H
#define IDISTORT_NODE_H

#include <QPainter>
#include <node_engine.h>

class idistort_node : public node_engine
{
public:
    idistort_node();
    ~idistort_node();

    void render( render_data *rdata, QJsonObject *params ) override;
};

#endif // IDISTORT_NODE_H
