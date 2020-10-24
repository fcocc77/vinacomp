#include <node.hpp>

node::node(QGraphicsScene *_scene)
{

    scene = _scene;

    inputs = new QList<QGraphicsLineItem *>;

    label = new QLabel("Nodo");

    width = 100;
    height = 30;

    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    this->setRect(0, 0, width, height);

    QGraphicsProxyWidget *proxy = scene->addWidget(label);

    QPen pen(Qt::green);
    pen.setWidth(3);
    this->setPen(pen);

    scene->addItem(this);
    add_input();

    // line->setParentItem(this);
    proxy->setParentItem(this);
}

node::~node()
{
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