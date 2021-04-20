#include "node_rect.h"

node_rect::node_rect(node_props _props, QMap<QString, node *> *_selected_nodes,
                     QWidget *_node_graph)

    : node(_props, _selected_nodes, _node_graph)
    , icon_area_width(45)
{
    this->setFlags(QGraphicsItem::ItemIsMovable);
    node::set_size(150, 50);

    // Texto
    {
        name_text = new QGraphicsTextItem;
        QFont font;
        font.setPointSize(15);
        name_text->setFont(font);
        name_text->setParentItem(this);
        name_text->setDefaultTextColor(Qt::black);

        tips_text = new QGraphicsTextItem;
        QFont font_tips;
        font_tips.setPointSize(10);
        tips_text->setFont(font_tips);
        tips_text->setParentItem(this);
    }
    //
    //

    // Rectangulo Forma
    {
        change_size_rectangle(minimum_width, minimum_height);

        QPen pen(Qt::black);
        QLinearGradient ramp(0, 0, icon_area_width * 2, 0);
        ramp.setColorAt(0.5000, QColor(50, 50, 50));
        ramp.setColorAt(0.5001, get_color());

        QBrush brush(ramp);
        pen.setWidth(0);
        this->setBrush(brush);
        this->setPen(pen);
    }
    //
    //

    set_name(name);
    set_tips(tips);
    set_icon(nodes_loaded->get_effect(type).value("icon").toString());
}

node_rect::~node_rect() {}

void node_rect::set_name(QString _name)
{
    name_text->setPlainText(_name);

    int text_width = name_text->boundingRect().width();
    int new_width = text_width + icon_area_width;

    if (new_width < minimum_width)
        new_width = minimum_width;

    // centra texto al area de texto
    int text_area = new_width - icon_area_width;
    int text_pos_x = (text_area - text_width) / 2;

    name_text->setPos(icon_area_width + text_pos_x, 0);
    //
    //

    change_size_rectangle(new_width, current_height);

    node::set_name(_name);
}

void node_rect::set_tips(QString _tips)
{
    tips_text->setPlainText(_tips);
    tips_text->setPos(60, 20);

    node::set_tips(_tips);
}

void node_rect::set_icon(QString _icon_name)
{
    icon_name = _icon_name;
    QImage image("resources/images/" + icon_name + "_a.png");
    QPixmap icon = QPixmap::fromImage(image);
    icon = icon.scaledToHeight(40, Qt::SmoothTransformation);
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(icon);
    item->setPos(2, 5);
    item->setParentItem(this);

    node::set_icon_name(_icon_name);
}

void node_rect::change_size_rectangle(int _width, int _height)
{
    if (_width < minimum_width)
        _width = minimum_width;

    current_width = _width;

    int radius = 3;
    QPainterPath rectangle;
    rectangle.addRoundedRect(QRectF(0, 0, _width, _height), radius, radius);
    this->setPath(rectangle);
}
