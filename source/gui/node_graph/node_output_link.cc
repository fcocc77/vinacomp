#include <node_output_link.h>
#include <util.h>

output_link::output_link(QGraphicsScene *scene) {

    QPen rectangle_pen(Qt::red);
    rectangle_pen.setWidth(1);
    this->setPen(rectangle_pen);

    scene->addItem(this);
}

output_link::~output_link() {}

void output_link::refresh()
{
    // print("refresh outputlink");
}
