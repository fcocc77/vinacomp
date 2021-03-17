#ifndef GRADE_NODE_H
#define GRADE_NODE_H

#include <QPainter>
#include <node_engine.h>

#include <opencv2/opencv.hpp>

class grade_node : public node_engine
{
public:
    grade_node();
    ~grade_node();

    void render( render_data *rdata, QJsonObject *params ) override;
};

#endif // GRADE_NODE_H
