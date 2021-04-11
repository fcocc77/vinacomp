#include "./node.h"
#include <panels_layout.h>
#include <vinacomp.h>
#include <util.h>

node::node(node_props _props, QMap<QString, node *> *_selected_nodes)

    : props(_props)
    , selected_nodes(_selected_nodes)
    , nodes_loaded(_props.nodes_loaded)

    , _trim_panel(nullptr)
    , _viewer(nullptr)
    , links(nullptr)

{
    nodes_connected_to_the_output = new QMap<QString, node *>;
    nodes_connected_to_the_inputs = new QMap<QString, node *>;
    center_position = new QPointF;

    name = props.name;
    tips = props.name;
    type = props.type;
    color = props.color;

    props.scene->addItem(this);

    if (type == "backdrop")
        this->setZValue(-100);
    else
    {
        // Crea los links para el nodo
        links = new QList<node_link *>;
        for (int i = 0; i < props.inputs; i++)
        {
            node_link *link =
                new node_link(i, props.scene, this, props.link_connecting,
                              props.project, props.vinacomp);
            links->push_back(link);
        }
        //

        this->setZValue((*props.current_z_value) + 1);
    }
}

node::~node() {}

void node::set_size(int _minimum_width, int _minimum_height)
{
    minimum_width = _minimum_width;
    minimum_height = _minimum_height;

    current_width = minimum_width;
    current_height = minimum_height;
}

void node::make_panel()
{
    QString name = get_name();

    // Crear panel de 'knobs'
    QStringList nodes_without_panel = {"viewer", "dot", "backdrop"};

    if (!nodes_without_panel.contains(type))
    {
        if (!_trim_panel)
            _trim_panel =
                new trim_panel(props._properties, name, type, color, icon_name,
                               nodes_loaded, props.project, props.vinacomp);
        props._properties->add_trim_panel(_trim_panel);
    }
    //
    //

    // Viewer
    vinacomp *__vinacomp = static_cast<vinacomp *>(props.vinacomp);
    if (type == "viewer")
    {
        if (!_viewer)
        {
            _viewer =
                new viewer(name, props.project, __vinacomp->get_renderer(), props.vinacomp);

            __vinacomp->get_viewers()->push_back(_viewer);
        }
        __vinacomp->get_panels_layout()->add_viewer(_viewer);
    }
    //
}

void node::refresh()
{
    if (!links)
        return;

    // Actualizacion de todos lo links conectados al nodo
    auto refresh_links = [](node *_node) {
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

    if (links)
        for (node_link *link : *links)
            link->set_selected(enable);
}

void node::set_name(QString _name)
{
    this->setData(0, _name);

    name = _name;

    if (_trim_panel)
        _trim_panel->set_name(_name);
}

void node::set_position(float x, float y)
{
    this->setPos(x, y);
    this->refresh();
}

void node::set_position(QPointF position)
{
    this->setPos(position);
    this->refresh();
}

QPointF node::get_center_position() const
{
    float x = this->x() + current_width / 2;
    float y = this->y() + current_height / 2;

    *center_position = {x, y};

    return *center_position;
}

void node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    make_panel();
}

void node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (type != "backdrop")
    {
        // con esto se mantiene siempre este nodo sobre los demas
        (*props.current_z_value)++;
        this->setZValue(*props.current_z_value);
    }

    freeze_position();
    click_position = mapToScene(event->pos());

    selected_nodes_start_position.clear();
    for (node *selected_node : *selected_nodes)
        selected_nodes_start_position[selected_node->get_name()] =
            selected_node->pos();
}

void node::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    int snap = 20;

    QPointF position = mapToScene(event->pos());
    QPointF click_position_on_node = click_position - _freeze_position;

    float this_node_x = position.x() - click_position_on_node.x();
    float this_node_y = position.y() - click_position_on_node.y();

    float x_snap = NULL;
    float y_snap = NULL;

    auto to_snap = [&](node *connected_node) {
        if (selected_nodes->contains(connected_node->get_name()))
            return;

        float size_x_difference =
            (connected_node->get_size().width() - this->get_size().width()) / 2;
        float size_y_difference =
            (connected_node->get_size().height() - this->get_size().height()) /
            2;

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
    QPointF difference = _freeze_position - position_with_snap;
    for (node *selected_node : *selected_nodes)
    {
        if (selected_node != this)
        {
            QPointF new_position =
                selected_nodes_start_position.value(selected_node->get_name()) -
                difference;
            selected_node->set_position(new_position.x(), new_position.y());
        }
    }
    //
    //
}
