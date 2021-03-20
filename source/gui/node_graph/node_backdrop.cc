#include "node_backdrop.h"
#include "node_view.h"

node_backdrop::node_backdrop(node_props _props,
                             QMap<QString, node *> *_selected_nodes,
                             QWidget *__node_view)

    : node(_props, _selected_nodes)
    , props(_props)
    , _node_view(__node_view)
{
    this->setFlags(QGraphicsItem::ItemIsMovable);
    set_size(500, 300);

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

    QLinearGradient ramp(0, 40, 0, 0);
    ramp.setColorAt(0.5000, get_color());
    ramp.setColorAt(0.5001, QColor(200, 200, 200));

    QBrush brush(ramp);
    pen.setWidth(0);
    this->setBrush(brush);
    this->setPen(pen);
    //
    //

    node::set_name(_props.name);
    set_tips("hola a todos");
}

node_backdrop::~node_backdrop() {}

void node_backdrop::set_tips(QString _tips)
{
    tips_text->setPlainText(_tips);
    tips_text->setPos(20, 0);

    node::set_tips(_tips);
}

void node_backdrop::change_size_rectangle(int _width, int _height)
{
    if (_width < minimum_width)
        _width = minimum_width;

    current_width = _width;

    int radius = 3;
    QPainterPath rectangle;
    rectangle.addRoundedRect(QRectF(0, 0, _width, _height), radius, radius);
    this->setPath(rectangle);
}

void node_backdrop::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // encuentra todos los nodos que estan dentro del 'backdrop' para
    // luego arrastrarlos junto con el 'backdrop'

    QPainterPath rectangle;

    QRectF rect(this->x(), this->y(), current_width, current_height);
    rectangle.addRect(rect);

    node_view *__node_view = static_cast<node_view *>(_node_view);
    nodes_to_drag.clear();

    QList<QGraphicsItem *> selected_items = props.scene->items(rectangle);
    for (QGraphicsItem *item : selected_items)
    {
        QString node_name = item->data(0).toString();
        node *_node = __node_view->get_node(node_name);
        if (_node)
        {
            if (_node->get_type() != "backdrop")
            {
                _node->freeze_position();
                nodes_to_drag.push_back(_node);
            }
        }
    }

    node::mousePressEvent(event);
}

void node_backdrop::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // calcula el movimiento del backdrop, restandolo con la posicion,
    // para que el movimiento inicie en 0, y asi sumarlo a los nodos
    QPointF position = mapToScene(event->pos());
    QPointF click_position_backdrop =
        node::click_position - node::get_freeze_position();

    QPointF new_position_backdrop = position - click_position_backdrop;
    QPointF add_position = new_position_backdrop - node::get_freeze_position();
    //

    for (node *_node : nodes_to_drag)
        _node->set_position(_node->get_freeze_position() + add_position);

    node::mouseMoveEvent(event);
}
