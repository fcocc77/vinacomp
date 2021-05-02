#include "node_rect.h"

node_rect::node_rect(node_props _props, QMap<QString, node *> *_selected_nodes,
                     QWidget *_node_graph)

    : node(_props, _selected_nodes, _node_graph)
    , icon_area_width(45)
    , disable(false)
    , attributes(false)
    , animated(false)
    , cloned(false)
    , expression(false)
    , with_link(false)

{
    this->setFlags(QGraphicsItem::ItemIsMovable);

    // Texto
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
    //
    //

    // caja de atributos
    attribute_box = new QGraphicsPathItem;
    attribute_box->setVisible(attributes);
    QLinearGradient attr_ramp(0, 100, 0, 0);
    attr_ramp.setColorAt(0.5, QColor(40, 40, 40));
    attr_ramp.setColorAt(0.45, QColor(55, 55, 55));
    attribute_box->setBrush(attr_ramp);
    attribute_box->setPen(QPen(Qt::black, 0));
    _props.scene->addItem(attribute_box);

    anim_text = new QGraphicsTextItem;
    clone_text = new QGraphicsTextItem;
    expression_text = new QGraphicsTextItem;
    link_text = new QGraphicsTextItem;

    anim_item = add_attribute_item("A", anim_text);
    clone_item = add_attribute_item("C", clone_text);
    expression_item = add_attribute_item("E", expression_text);
    link_item = add_attribute_item("L", link_text);

    anim_item->setParentItem(attribute_box);
    clone_item->setParentItem(attribute_box);
    expression_item->setParentItem(attribute_box);
    link_item->setParentItem(attribute_box);
    //

    set_icon(nodes_loaded->get_effect(type).value("icon").toString());

    // lineas de 'disable'
    disable_line_a = new QGraphicsLineItem;
    disable_line_b = new QGraphicsLineItem;

    disable_line_a->setPen(QPen(Qt::black, 0));
    disable_line_b->setPen(QPen(Qt::black, 0));

    disable_line_a->setParentItem(this);
    disable_line_b->setParentItem(this);
    if (_props.from_project)
        disable = _props.project->nodes.value(_props.name)
                      .params->value("disable_node")
                      .toBool();
    set_disable(disable);
    //

    // Rectangulo Forma
    QPen pen(Qt::black);
    QLinearGradient ramp(0, 0, icon_area_width * 2, 0);
    ramp.setColorAt(0.5000, QColor(50, 50, 50));
    ramp.setColorAt(0.5001, get_color());

    QBrush brush(ramp);
    pen.setWidth(0);
    this->setBrush(brush);
    this->setPen(pen);
    //
    //

    set_minimum_size(150, 50);
    set_size(150, 50);

    set_name(name);
    set_tips(tips);

    refresh_attribute_items();
}

node_rect::~node_rect()
{
    delete name_text;
    delete tips_text;
    delete disable_line_a;
    delete disable_line_b;
    delete icon_item;

    delete anim_text;
    delete clone_text;
    delete expression_text;
    delete link_text;

    delete anim_item;
    delete clone_item;
    delete expression_item;
    delete link_item;

    delete attribute_box;
}

void node_rect::refresh_attribute_items()
{
    QColor disable_color = {45, 45, 45};
    if (animated)
    {
        anim_item->setBrush(QBrush(Qt::cyan));
        anim_text->setVisible(true);
    }
    else
    {
        anim_item->setBrush(QBrush(disable_color));
        anim_text->setVisible(false);
    }

    if (cloned)
    {
        clone_text->setVisible(true);
        clone_item->setBrush(QBrush(Qt::magenta));
    }
    else
    {
        clone_text->setVisible(false);
        clone_item->setBrush(QBrush(disable_color));
    }

    if (expression)
    {
        expression_text->setVisible(true);
        expression_item->setBrush(QBrush(Qt::green));
    }
    else
    {
        expression_text->setVisible(false);
        expression_item->setBrush(QBrush(disable_color));
    }

    if (with_link)
    {
        link_text->setVisible(true);
        link_item->setBrush(QBrush(Qt::cyan));
    }
    else
    {
        link_text->setVisible(false);
        link_item->setBrush(QBrush(disable_color));
    }

    attributes = animated || cloned || expression || with_link;
    attribute_box->setVisible(attributes);

    refresh_attribute_box();
}

void node_rect::refresh_attribute_box()
{
    if (!attributes)
        return;

    int width = current_width;
    int height = current_height;

    // caja
    int radius = 3;
    QPainterPath attr_box_rect;
    int attr_width = 70;
    int attr_height = 25;
    float attr_pos_x = (width - attr_width) / 2;
    attr_box_rect.addRoundedRect(
        QRectF(attr_pos_x, height - 10, attr_width, attr_height), radius, radius);
    attribute_box->setPath(attr_box_rect);
    //

    float space = 8;
    for (QGraphicsItem *item :
         {anim_item, clone_item, expression_item, link_item})
    {
        item->setPos({space + attr_pos_x, height + 3.0});
        space += 15;
    }
}

QGraphicsPathItem *node_rect::add_attribute_item(QString letter,
                                                 QGraphicsTextItem *text_item)
{
    QGraphicsPathItem *item = new QGraphicsPathItem;
    item->setPen(QPen(Qt::black, 0));

    QFont font;
    font.setPixelSize(7);
    font.setBold(true);
    text_item->setFont(font);
    text_item->setParentItem(item);
    text_item->setDefaultTextColor(Qt::black);
    text_item->setPlainText(letter);
    text_item->setPos(-1.5, -3.5);

    int size = 10;
    QPainterPath _path;
    _path.addRoundedRect(QRectF(0, 0, size, size), size, size);
    item->setPath(_path);

    return item;
}

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
    if (icon_name.contains("/"))
        image = QImage(icon_name);

    QPixmap icon = QPixmap::fromImage(image);
    icon = icon.scaledToHeight(40, Qt::SmoothTransformation);
    icon_item = new QGraphicsPixmapItem(icon);
    icon_item->setPos(2, 5);
    icon_item->setParentItem(this);

    node::set_icon_name(_icon_name);
}

void node_rect::set_position(float x, float y)
{
    attribute_box->setPos(x, y);
    node::set_position(x, y);
}

void node_rect::set_selected(bool selected)
{
    if (selected)
    {
        disable_line_a->setPen(QPen(Qt::white, 2));
        disable_line_b->setPen(QPen(Qt::white, 2));
    }
    else
    {
        disable_line_a->setPen(QPen(Qt::black, 0));
        disable_line_b->setPen(QPen(Qt::black, 0));
    }
    node::set_selected(selected);
}

void node_rect::refresh()
{
    int width = current_width;
    int height = current_height;

    float radius = 3;
    QPainterPath rectangle;
    rectangle.addRoundedRect(QRectF(0, 0, width, height), radius, radius);
    this->setPath(rectangle);

    refresh_attribute_box();

    // disable lines
    if (!disable)
        return;

    disable_line_a->setLine(
        {{0, radius}, {(float)width, (float)height - radius}});
    disable_line_b->setLine(
        {{0, (float)height - radius}, {(float)width, radius}});
    //
}

void node_rect::set_size(int width, int height)
{
    if (height < minimum_height)
        height = minimum_height;

    if (width < minimum_width)
        width = minimum_width;

    node::set_size(width, height);
    refresh();
}

void node_rect::set_name(QString _name)
{
    update_text(_name, get_tips());
    node::set_name(_name);
}

void node_rect::set_disable(bool _disable)
{
    disable = _disable;
    disable_line_a->setVisible(disable);
    disable_line_b->setVisible(disable);
    refresh();
}

void node_rect::set_tips(QString _tips)
{
    update_text(get_name(), _tips);
    node::set_tips(_tips);
}
