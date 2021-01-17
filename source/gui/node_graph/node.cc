#include <node.h>
#include <vinacomp.h>
#include <panels_layout.h>

node::node(QGraphicsScene *_scene,
           int *_current_z_value,
           QJsonObject *_link_connecting,
           QMap<QString, node *> *_selected_nodes,
           int inputs,
           QColor _color,
		   QString _type,
           trim_panel *__trim_panel,
		   viewer *__viewer,
           properties *__properties,
		   QWidget *__vinacomp)

    : _trim_panel(__trim_panel)
	, _viewer(__viewer)
	, _properties(__properties)
	, _vinacomp(__vinacomp)
	, color(_color)
	, type(_type)
	, scene(_scene)
	, current_z_value(_current_z_value)
	, selected_nodes(_selected_nodes)

	, minimum_width(150)
	, minimum_height(50)
	, icon_area_width(45)

{
    center_position = new QPointF;
    nodes_connected_to_the_output = new QMap<QString, node *>;
    nodes_connected_to_the_inputs = new QMap<QString, node *>;

    current_width = minimum_width;
    current_height = minimum_height;

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
        name->setDefaultTextColor(Qt::black);

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
        ramp.setColorAt(0.5001, color);

        QBrush brush(ramp);
        pen.setWidth(0);
        this->setBrush(brush);
        this->setPen(pen);
    }
    //
    //

    // Crea los links para el nodo
    {
        links = new QList<node_link *>;
        for (int i = 0; i < inputs; i++)
        {
            node_link *link = new node_link(i, scene, this, _link_connecting);
            links->push_back(link);
        }
    }
    //
    //

    scene->addItem(this);

    this->setZValue((*current_z_value) + 1);
}

node::~node()
{
}

void node::refresh()
{
    // Actualizacion de todos lo links conectados al nodo
    auto refresh_links = [this](node *_node) {
        for (node_link *_node_link : *_node->get_links())
            _node_link->refresh();
    };

    refresh_links(this);
    // refresca los link de cada nodo seleccionado y los
    // link de los nodos que estan conectados a la salida.
    for (node *output_node : *this->get_output_nodes())
        refresh_links(output_node);
    //
    //
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
    selected = enable;
    if (enable)
    {
        QPen pen(Qt::white);
        pen.setWidth(3);
        this->setPen(pen);
    }
    else
    {
        QPen pen(Qt::black);
        pen.setWidth(0);
        this->setPen(pen);
    }

    for (node_link *link : *links)
        link->set_selected(enable);
}

bool node::is_selected() const
{
    return selected;
}

QPointF node::get_center_position() const
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

	if (_trim_panel)
		_trim_panel->set_name(_name);
}

QString node::get_name() const
{
    return name->toPlainText();
}

void node::set_tips(QString _tips)
{
    tips->setPlainText(_tips);
    tips->setPos(60, 20);
}

QSize node::get_size() const
{
    return QSize(current_width, current_height);
}

trim_panel *node::get_trim_panel() const
{
	return _trim_panel;
}

QColor node::get_color() const
{
    return color;
}

void node::set_position(float x, float y)
{
    this->setPos(x, y);
    this->refresh();
}

void node::add_output_node(node *_node)
{
    nodes_connected_to_the_output->insert(_node->get_name(), _node);
}

void node::remove_output_node(node *_node)
{
    nodes_connected_to_the_output->remove(_node->get_name());
}

QMap<QString, node *> *node::get_output_nodes() const
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

QList<node_link *> *node::get_links() const
{
    return links;
}

QString node::get_type() const
{
	return type;
}

void node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	if (_trim_panel)
		_properties->add_trim_panel(_trim_panel);

	if (_viewer)
	{
		panels_layout *_panels_layout = dynamic_cast<vinacomp*>(_vinacomp)->get_panels_layout();
		_panels_layout->add_viewer(_viewer);
	}
}

void node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // con esto se mantiene siempre este nodo sobre los demas
    (*current_z_value)++;
    this->setZValue(*current_z_value);
    //
    //

    start_position = this->pos();
    click_position = mapToScene(event->pos());

    selected_nodes_start_position.clear();
    for (node *selected_node : *selected_nodes)
        selected_nodes_start_position[selected_node->get_name()] = selected_node->pos();
}

void node::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    int snap = 20;

    QPointF position = mapToScene(event->pos());
    QPointF click_position_on_node = click_position - start_position;

    float this_node_x = position.x() - click_position_on_node.x();
    float this_node_y = position.y() - click_position_on_node.y();

    float x_snap = NULL;
    float y_snap = NULL;

    auto to_snap = [&](node *connected_node) {
        if (selected_nodes->contains(connected_node->get_name()))
            return;

        float size_x_difference = (connected_node->get_size().width() - this->get_size().width()) / 2;
        float size_y_difference = (connected_node->get_size().height() - this->get_size().height()) / 2;

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

    QPointF position_with_snap;

    if (x_snap && y_snap)
        position_with_snap = {x_snap, y_snap};
    else if (x_snap)
        position_with_snap = {x_snap, this_node_y};
    else if (y_snap)
        position_with_snap = {this_node_x, y_snap};
    else
        position_with_snap = {this_node_x, this_node_y};

    this->set_position(position_with_snap.x(), position_with_snap.y());
    //
    //

    // Mueve los nodos seleccionados en relacion a este nodo
    QPointF difference = start_position - position_with_snap;
    for (node *selected_node : *selected_nodes)
    {
        if (selected_node != this)
        {
            QPointF new_position = selected_nodes_start_position.value(selected_node->get_name()) - difference;
            selected_node->set_position(new_position.x(), new_position.y());
        }
    }
    //
    //
}
