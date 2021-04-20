#ifndef NODE_OUTPUT_LINK_H
#define NODE_OUTPUT_LINK_H

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QPen>

class output_link : public QGraphicsRectItem
{
public:
    output_link(QGraphicsScene *scene);
    ~output_link();

    void refresh();
};

#endif // NODE_OUTPUT_LINK_H
