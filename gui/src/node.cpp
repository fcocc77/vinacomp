#include <node.hpp>

node::node(QGraphicsScene *_scene,
           QList<node *> *_selected_nodes,
           int *_current_z_value)
{
    scene = _scene;
    selected_nodes = _selected_nodes;
    current_z_value = _current_z_value;

    links = new QList<node_link *>;
    connected_nodes = new QList<node *>;
    center_position = new QPoint;

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

        tips = new QGraphicsTextItem;
        QFont font_tips;
        font_tips.setPointSize(10);
        tips->setFont(font_tips);
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
    add_link();

    this->setZValue((*current_z_value) + 1);
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

        links->value(0)->setPen(pen);

        selected_nodes->push_back(this);

        // con esto se mantiene siempre el nodo seleccionado sobre los demas
        (*current_z_value)++;
        this->setZValue(*current_z_value);
    }
    else
    {
        QPen pen(Qt::black);
        pen.setWidth(1);
        this->setPen(pen);

        QPen pen_input(Qt::black);
        pen_input.setWidth(25);
        links->value(0)->setPen(pen_input);
    }
}

void node::refresh()
{
    int input_index = 0;
    input_line_connect(input_index);
}

QPoint node::get_center_position()
{
    int x = this->x() + current_width / 2;
    int y = this->y() + current_height / 2;

    *center_position = {x, y};

    return *center_position;
}

void node::input_line_connect(int index)
{
    node *connected_node = connected_nodes->value(index);
    QPoint src_pos, dst_pos;

    src_pos = this->get_center_position();
    if (connected_node != NULL)
        dst_pos = connected_node->get_center_position();
    else
        dst_pos = {src_pos.x(), src_pos.y() - 50};

    links->value(index)->setLine(src_pos.x(), src_pos.y(), dst_pos.x(), dst_pos.y());
}

void node::add_link()
{

    QPen pen(Qt::black);
    pen.setWidth(2);
    node_link *input = new node_link(center_position);

    scene->addItem(input);

    links->push_back(input);
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

void node::set_tips(QString _tips)
{
    tips->setPlainText(_tips);
    tips->setPos(60, 20);
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
