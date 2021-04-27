#include "node_rect.h"

node_rect::node_rect(node_props _props, QMap<QString, node *> *_selected_nodes,
                     QWidget *_node_graph)

    : node(_props, _selected_nodes, _node_graph)
    , icon_area_width(45)
{
    this->setFlags(QGraphicsItem::ItemIsMovable);
    set_minimum_size(150, 50);
    set_size(150, 50);

    // Texto
    {
        name_text = new QGraphicsTextItem;
        QFont font;
        font.setPixelSize(23);
        name_text->setFont(font);
        name_text->setParentItem(this);
        name_text->setDefaultTextColor(Qt::black);

        tips_text = new QGraphicsTextItem;
        QFont font_tips;
        font_tips.setPixelSize(14);
        tips_text->setFont(font_tips);
        tips_text->setParentItem(this);
    }
    //
    //

    // Rectangulo Forma
    {
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

void node_rect::update_text(QString _name, QString _tips)
{
    name_text->setPlainText(_name);
    tips_text->setPlainText(_tips);

    // Name Ajuste
    int name_width = name_text->boundingRect().width();
    int name_height = name_text->boundingRect().height();

    int new_width = name_width + icon_area_width;

    if (new_width < minimum_width)
        new_width = minimum_width;

    // centra texto al area de texto
    int text_area = new_width - icon_area_width;
    int text_pos_x = (text_area - name_width) / 2;

    // si el tips esta vacio, centra el texto al alto minimo
    int text_pos_y = 0;
    if (_tips.isEmpty())
        text_pos_y = (minimum_height - name_height) / 2;
    //

    name_text->setPos(icon_area_width + text_pos_x, text_pos_y);
    //
    //

    if (!_tips.isEmpty())
    {
        tips_text->setPos(name_text->x(), name_height);

        // default 'tips_text'
        tips_text->setTextWidth(-1);
        int tips_width = tips_text->boundingRect().width();
        int tips_height = tips_text->boundingRect().height();
        //

        if (tips_width > name_width)
        {
            // el ancho del 'tips' puede ser 1.5 mas grande que el nombre
            tips_text->setTextWidth(name_width * 1.5);

            tips_width = tips_text->boundingRect().width();
            tips_height = tips_text->boundingRect().height();

            new_width = tips_text->x() + tips_width;
        }
        else
        {
            // si el tips es mas chico que el nombre, los acerca en eje 'y' para
            // que no esten tan separados y centra el tips al nombre
            name_text->setPos(name_text->x(), name_text->y() + 5);
            int tips_y = tips_text->x() + (name_width / 2) - (tips_width / 2);
            tips_text->setPos(tips_y, tips_text->y() - 5);
        }

        set_size(new_width, tips_height + name_height);
    }
    else
        set_size(new_width, 0);
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

void node_rect::set_size(int width, int height)
{
    if (height < minimum_height)
        height = minimum_height;

    if (width < minimum_width)
        width = minimum_width;

    node::set_size(width, height);

    int radius = 3;
    QPainterPath rectangle;
    rectangle.addRoundedRect(QRectF(0, 0, width, height), radius, radius);
    this->setPath(rectangle);
}

void node_rect::set_name(QString _name)
{
    update_text(name, get_tips());
    node::set_name(_name);
}

void node_rect::set_tips(QString _tips)
{
    update_text(get_name(), _tips);
    node::set_tips(_tips);
}
