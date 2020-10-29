#include <node.hpp>

node::node(QGraphicsScene *_scene, QList<node *> *_selected_nodes)
{
    scene = _scene;
    selected_nodes = _selected_nodes;

    inputs = new QList<QGraphicsLineItem *>;
    connected_nodes = new QList<node *>;

    selected = false;

    minimum_width = 150;
    minimum_height = 50;
    current_width = minimum_width;
    current_height = minimum_height;

    icon_area_width = 45;

    this->setFlags(QGraphicsItem::ItemIsMovable);
    //
    //

    // Texto
    {
        name = new QGraphicsTextItem;

        QFont font;
        font.setPointSize(15);
        name->setFont(font);
        name->setParentItem(this);

        QGraphicsTextItem *tips = new QGraphicsTextItem;

        QFont font2;
        font.setPointSize(5);
        tips->setPlainText("este es el tool tips");
        tips->setFont(font2);
        tips->setPos(50, 20);
        tips->setParentItem(this);
    }
    //
    //

    // Rectangulo Forma
    {
        change_size_rectangle(minimum_width, minimum_height);

        QPen pen(Qt::black);
        QLinearGradient ramp(0, 0, icon_area_width * 2, 0);
        ramp.setColorAt(0.5000, QColor(50, 50, 50));
        ramp.setColorAt(0.5001, QColor(150, 150, 150));

        QBrush color(ramp);
        pen.setWidth(1);
        this->setBrush(color);
        this->setPen(pen);
    }
    //
    //

    scene->addItem(this);
    add_input();

    this->setZValue(2);
}

node::~node()
{
}

void node::set_icon(QString icon_name)
{
    QImage image("resources/images/" + icon_name + ".png");
    QPixmap icon = QPixmap::fromImage(image);
    icon = icon.scaledToHeight(40, Qt::SmoothTransformation);
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(icon);
    item->setPos(2, 5);
    item->setParentItem(this);
}

void node::change_size_rectangle(int _width, int _height)
{
    if (_width < minimum_width)
        _width = minimum_width;

    current_width = _width;

    int radius = 3;
    QPainterPath rectangle;
    rectangle.addRoundedRect(QRectF(0, 0, _width, _height), radius, radius);
    this->setPath(rectangle);
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
        QPen pen(Qt::white);
        pen.setWidth(3);
        this->setPen(pen);

        inputs->value(0)->setPen(pen);

        selected_nodes->push_back(this);
    }
    else
    {
        QPen pen(Qt::black);
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

    src_x = this->x() + current_width / 2;
    src_y = this->y() + current_height / 2;

    if (connected_node != NULL)
    {
        dst_x = connected_node->x() + connected_node->get_size().value(0) / 2;
        dst_y = connected_node->y() + connected_node->get_size().value(1) / 2;
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

void node::set_name(QString _name)
{
    name->setPlainText(_name);

    int text_width = name->boundingRect().width();
    int new_width = text_width + icon_area_width;

    if (new_width < minimum_width)
        new_width = minimum_width;

    // centra texto al area de texto
    int text_area = new_width - icon_area_width;
    int text_pos_x = (text_area - text_width) / 2;

    name->setPos(icon_area_width + text_pos_x, 0);
    //
    //

    change_size_rectangle(new_width, current_height);
}

QString node::get_name()
{
    return name->toPlainText();
}

void node::set_tips(QString tips)
{
}

QList<int> node::get_size()
{
    return {current_width, current_height};
}

void node::set_position(int x, int y)
{
    this->setPos(x, y);

    input_line_connect(0);
}