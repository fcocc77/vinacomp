#include <node.hpp>

node::node(QGraphicsScene *_scene,
           int *_current_z_value)
{
    scene = _scene;
    current_z_value = _current_z_value;

    center_position = new QPointF;
    nodes_connected_to_the_output = new QMap<QString, node *>;

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

void node::set_selected_style(bool enable)
{
    if (enable)
    {
        QPen pen(Qt::white);
        pen.setWidth(3);
        this->setPen(pen);

        // con esto se mantiene siempre el nodo seleccionado sobre los demas
        (*current_z_value)++;
        this->setZValue(*current_z_value);
    }
    else
    {
        QPen pen(Qt::black);
        pen.setWidth(1);
        this->setPen(pen);
    }
}

QPointF node::get_center_position()
{
    float x = this->x() + current_width / 2;
    float y = this->y() + current_height / 2;

    *center_position = {x, y};

    return *center_position;
}

void node::set_name(QString _name)
{
    this->setData(0, _name);
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
}

void node::add_output_node(node *_node)
{
    nodes_connected_to_the_output->insert(_node->get_name(), _node);
}

void node::remove_output_node(node *_node)
{
    nodes_connected_to_the_output->remove(_node->get_name());
}

QMap<QString, node *> *node::get_output_nodes()
{
    return nodes_connected_to_the_output;
}