#include <node.hpp>

node::node(QGraphicsScene *_scene, QList<node *> *_nodes)
{
    scene = _scene;
    nodes = _nodes;

    inputs = new QList<QGraphicsLineItem *>;

    label = new QLabel("Nodo");
    selected = false;

    width = 100;
    height = 30;

    this->setFlags(QGraphicsItem::ItemIsMovable);
    this->setRect(0, 0, width, height);

    QGraphicsProxyWidget *proxy = scene->addWidget(label);

    QPen pen(Qt::white);
    pen.setWidth(1);
    this->setPen(pen);

    scene->addItem(this);
    add_input();

    proxy->setParentItem(this);
}

node::~node()
{
}

void node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // deseleccionar todos los nodos antes de seleccionar este
    for (node *_node : *nodes)
        _node->select(false);

    this->select(true);
}

void node::select(bool _selected)
{
    selected = _selected;

    if (selected)
    {
        QPen pen(Qt::green);
        pen.setWidth(3);
        this->setPen(pen);

        inputs->value(0)->setPen(pen);
    }
    else
    {
        QPen pen(Qt::white);
        pen.setWidth(1);
        this->setPen(pen);

        QPen pen_input(Qt::black);
        inputs->value(0)->setPen(pen_input);
    }
}

void node::inputs_refresh()
{
    // actualiza la posicion de las lineas

    inputs->value(0)->setLine(0, 0, 200, 50);
}

void node::add_input()
{

    QPen pen(Qt::black);
    pen.setWidth(2);
    QGraphicsLineItem *input = scene->addLine(0, 0, 100, 50, pen);

    inputs->push_back(input);
}

QGraphicsLineItem node::get_input(int index)
{
}

void node::connect_input(int index, node *_node)
{

    int src_x = this->x() + width / 2;
    int src_y = this->y() + height / 2;

    int dst_x = _node->x() + width / 2;
    int dst_y = _node->y() + height / 2;

    inputs->value(index)->setLine(src_x, src_y, dst_x, dst_y);
}

void node::set_name(QString name)
{
    label->setText(name);
}

void node::set_position(int x, int y)
{
    this->setPos(x, y);

    int src_x = x + width / 2;
    int src_y = y + height / 2;

    int dst_x = src_x;
    int dst_y = src_y - 50;

    inputs->value(0)->setLine(src_x, src_y, dst_x, dst_y);
}