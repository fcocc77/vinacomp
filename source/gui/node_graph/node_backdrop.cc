#include "node_backdrop.h"
#include "node_view.h"

node_backdrop::node_backdrop(node_props _props,
                             QMap<QString, node *> *_selected_nodes,
                             QWidget *__node_view)

    : node(_props, _selected_nodes)
    , props(_props)
    , _node_view(__node_view)
    , title_area_height(50)
    , clicked_body_area(false)
    , resizing(false)
{
    int z_value = -100;

    this->setFlags(QGraphicsItem::ItemIsMovable);
    this->setZValue(z_value);
    set_size(500, 300);

    // Tips
    tips_text = new QGraphicsTextItem;
    QFont font_tips;
    font_tips.setPointSize(10);
    tips_text->setFont(font_tips);
    tips_text->setParentItem(this);
    //
    //

    // Forma de Backdrop
    change_size_rectangle(minimum_width, minimum_height);

    QPen pen(Qt::black);

    QColor color = get_random_color();
    QColor color2 = color;

    // baja la intensidad al color 2
    int h, s, l;
    color2.getHsl(&h, &s, &l);
    color2.setHsl(h, s, l * 0.9);
    //

    QLinearGradient ramp(0, title_area_height, 0, 0);
    ramp.setColorAt(0.5000, color);
    ramp.setColorAt(0.5001, color2);

    QBrush brush(ramp);
    pen.setWidth(0);
    this->setBrush(brush);
    this->setPen(pen);
    //
    //

    // esquina de reescalado
    corner = new QGraphicsLineItem;
    corner_size = {30, 30};
    corner->setZValue(z_value);
    corner->setPen(QPen(Qt::black, 0));
    corner->setLine(
        {{(float)corner_size.width(), 0}, {0, (float)corner_size.height()}});
    props.scene->addItem(corner);
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

void node_backdrop::set_selected(bool enable)
{
    if (clicked_body_area && enable)
        return;

    if (enable)
        corner->setPen(QPen(Qt::white, 2));
    else
        corner->setPen(QPen(Qt::black, 0));

    node::set_selected(enable);
}

void node_backdrop::set_position(float x, float y)
{
    node::set_position(x, y);
    refresh_corner();
}

void node_backdrop::refresh_corner()
{
    float cor_x = x() + get_size().width() - corner_size.width();
    float cor_y = y() + get_size().height() - corner_size.height();
    corner->setPos(cor_x, cor_y);
}

bool node_backdrop::is_under_selector(QRectF selector)
{
    // verifica si el backdrop esta dentro de un rectangulo, se usa para la
    // seleccion

    int x1 = this->x();
    int x2 = x1 + this->get_size().width();
    int y1 = this->y();
    int y2 = y1 + this->get_size().height();

    int sel_x1 = selector.x();
    int sel_x2 = sel_x1 + selector.width();
    int sel_y1 = selector.y();
    int sel_y2 = sel_y1 + selector.height();

    return sel_x1 < x1 && sel_x2 > x2 && sel_y1 < y1 && sel_y2 > y2;
}

QColor node_backdrop::get_random_color() const
{
    float gain = 0.55;
    int saturation = 80; // 0 - 255

    int red = std::rand() % saturation + (255 - saturation);
    int green = std::rand() % saturation + (255 - saturation);
    int blue = std::rand() % saturation + (255 - saturation);

    red *= gain;
    green *= gain;
    blue *= gain;

    return QColor(red, green, blue);
}

void node_backdrop::resize(QSize size)
{
    if (!resizing)
        return;

    int min = 150;

    float width = size.width();
    float height = size.height();

    if (width < min)
        width = min;
    if (height < min)
        height = min;

    set_size(width, height);
    change_size_rectangle(minimum_width, minimum_height);
    refresh_corner();
}

void node_backdrop::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // detecta si el click se hizo en la esquina de reescalado
    int width = get_size().width();
    int height = get_size().height();

    int width_less = width - corner_size.width();
    int height_less = height - corner_size.height();

    if (event->pos().x() > width_less && event->pos().y() > height_less)
    {
        corner_click_diff.setWidth(width - event->pos().x());
        corner_click_diff.setHeight(height - event->pos().y());
        resizing = true;
    }
    //

    clicked_body_area = event->pos().y() > (title_area_height / 2);
    if (clicked_body_area)
        return;

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

void node_backdrop::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    clicked_body_area = false;
    resizing = false;
    node::mouseReleaseEvent(event);
}

void node_backdrop::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // re escalado
    resize({(int)event->pos().x() + corner_click_diff.width(),
            (int)event->pos().y() + corner_click_diff.height()});
    //

    if (clicked_body_area)
        return;

    // calcula el movimiento del backdrop, restandolo con la posicion,
    // para que el movimiento inicie en 0, y asi sumarlo a los nodos
    QPointF position = mapToScene(event->pos());
    QPointF click_position_backdrop =
        node::click_position - node::get_freeze_position();

    QPointF new_position_backdrop = position - click_position_backdrop;
    QPointF add_position = new_position_backdrop - node::get_freeze_position();
    //

    node::mouseMoveEvent(event);

    // si el control esta presionado no arrastra los nodos
    if (qt::control())
        return;

    for (node *_node : nodes_to_drag)
    {
        QPointF pos = _node->get_freeze_position() + add_position;
        _node->set_position(pos.x(), pos.y());
    }
}
