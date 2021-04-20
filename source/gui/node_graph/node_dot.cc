#include "node_dot.h"

node_dot::node_dot(node_props _props, QMap<QString, node *> *_selected_nodes,
                   QWidget *_node_graph)

    : node(_props, _selected_nodes, _node_graph)
{
    this->setFlags(QGraphicsItem::ItemIsMovable);
    set_size(20, 20);

    // Tips
    tips_text = new QGraphicsTextItem;
    QFont font_tips;
    font_tips.setPointSize(10);
    tips_text->setFont(font_tips);
    tips_text->setParentItem(this);
    //
    //

    // Forma de Dot
    change_size_rectangle(minimum_width, minimum_height);

    QPen pen(Qt::black);

    QBrush brush(get_color());
    pen.setWidth(0);
    this->setBrush(brush);
    this->setPen(pen);
    //
    //

    node::set_name(_props.name);
    set_tips("hola a todos");
}

node_dot::~node_dot() {}

void node_dot::set_tips(QString _tips)
{
    tips_text->setPlainText(_tips);
    tips_text->setPos(20, 0);

    node::set_tips(_tips);
}

void node_dot::change_size_rectangle(int _width, int _height)
{
    if (_width < minimum_width)
        _width = minimum_width;

    current_width = _width;

    int radius = 10;
    QPainterPath rectangle;
    rectangle.addRoundedRect(QRectF(0, 0, _width, _height), radius, radius);
    this->setPath(rectangle);
}
