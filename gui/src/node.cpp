#include <node.hpp>

node::node(QGraphicsScene *_scene, QList<node *> *_selected_nodes)
{
    scene = _scene;
    selected_nodes = _selected_nodes;

    inputs = new QList<QGraphicsLineItem *>;
    connected_nodes = new QList<node *>;

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
    for (node *_node : *selected_nodes)
        _node->select(false);
    selected_nodes->clear();
    //

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

        selected_nodes->push_back(this);
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

void node::refresh()
{
    int input_index = 0;
    input_line_connect(input_index);
}

void node::input_line_connect(int index)
{
    node *connected_node = connected_nodes->value(index);

    int src_x, src_y, dst_x, dst_y;

    src_x = this->x() + width / 2;
    src_y = this->y() + height / 2;

    if (connected_node != NULL)
    {
        dst_x = connected_node->x() + width / 2;
        dst_y = connected_node->y() + height / 2;
    }
    else
    {
        dst_x = src_x;
        dst_y = src_y - 50;
    }

    inputs->value(index)->setLine(src_x, src_y, dst_x, dst_y);
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
    connected_nodes->push_back(_node);
}

void node::set_name(QString name)
{
    label->setText(name);
}

QString node::get_name()
{
    return label->text();
}

void node::set_position(int x, int y)
{
    this->setPos(x, y);

    input_line_connect(0);
}