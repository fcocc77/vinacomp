#include <node.hpp>

node::node(QGraphicsScene *_scene,
           int *_current_z_value,
           QMap<QString, node *> *_selected_nodes)
{
    scene = _scene;
    current_z_value = _current_z_value;
    selected_nodes = _selected_nodes;

    center_position = new QPointF;
    nodes_connected_to_the_output = new QMap<QString, node *>;
    nodes_connected_to_the_inputs = new QMap<QString, node *>;

    minimum_width = 150;
    minimum_height = 50;
    current_width = minimum_width;
    current_height = minimum_height;

    icon_area_width = 45;

    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
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
        pen.setWidth(0);
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

void node::set_selected(bool enable)
{
    this->setSelected(enable);
    selected = enable;
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
        pen.setWidth(0);
        this->setPen(pen);
    }
}

bool node::is_selected()
{
    return selected;
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

void node::set_position(float x, float y)
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

void node::add_input_node(node *_node)
{
    nodes_connected_to_the_inputs->insert(_node->get_name(), _node);
}

void node::remove_input_node(node *_node)
{
    nodes_connected_to_the_inputs->remove(_node->get_name());
}

void node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // con esto desabilitamos el borde de puntos que genera un item al estar seleccionado
    QStyleOptionGraphicsItem myOption(*option);
    myOption.state &= ~QStyle::State_Selected;
    QGraphicsPathItem::paint(painter, &myOption, widget);
    //
    //
}

void node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    click_position_on_node = mapToScene(event->pos()) - this->pos();
}

void node::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    int snap = 20;

    if (selected_nodes->count() <= 1)
    {
        QPointF position = mapToScene(event->pos());

        float this_node_x = position.x() - click_position_on_node.x();
        float this_node_y = position.y() - click_position_on_node.y();

        float x_snap = NULL;
        float y_snap = NULL;

        auto to_snap = [&](node *connected_node) {
            float size_x_difference = (connected_node->get_size()[0] - this->get_size()[0]) / 2;
            float size_y_difference = (connected_node->get_size()[1] - this->get_size()[1]) / 2;

            float _this_node_x = this_node_x - size_x_difference;
            float _this_node_y = this_node_y - size_y_difference;

            float x_difference = abs(connected_node->x() - _this_node_x);
            float y_difference = abs(connected_node->y() - _this_node_y);

            if (x_difference < snap)
                x_snap = connected_node->x() + size_x_difference;

            else if (y_difference < snap)
                y_snap = connected_node->y() + size_y_difference;
        };

        // busca el snap en cada nodo conectado
        for (node *connected_node : *nodes_connected_to_the_inputs)
            to_snap(connected_node);
        for (node *connected_node : *nodes_connected_to_the_output)
            to_snap(connected_node);
        //
        //

        if (x_snap && y_snap)
            this->setPos(x_snap, y_snap);
        else if (x_snap)
            this->setPos(x_snap, this_node_y);
        else if (y_snap)
            this->setPos(this_node_x, y_snap);
        else
            this->setPos(this_node_x, this_node_y);

        return;
    }

    QGraphicsItem::mouseMoveEvent(event);
}