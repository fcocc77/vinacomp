#include "./node.h"
#include <panels_layout.h>
#include <vinacomp.h>
#include <util.h>
#include <node_graph.h>
#include <node_view.h>
#include <link.h>

node::node(node_props _props, QMap<QString, node *> *_selected_nodes,
           QWidget *_node_graph)

    : _trim_panel(nullptr)
    , _viewer(nullptr)
    , _node_view(nullptr)
    , props(_props)
    , selected_nodes(_selected_nodes)
    , links(nullptr)
    , _output_link(nullptr)
    , nodes_loaded(_props.nodes_loaded)

{
    if (_node_graph)
        _node_view = static_cast<node_graph *>(_node_graph)->get_node_view();

    nodes_connected_to_the_output = new QMap<QString, node *>;
    nodes_connected_to_the_inputs = new QMap<QString, node *>;
    center_position = new QPointF;

    name = props.name;
    tips = props.tips;
    type = props.type;
    set_color(props.color);

    props.scene->addItem(this);

    if (type != "backdrop")
    {
        QJsonObject node_fx = nodes_loaded->get_effect(props.type);
        // carga las entradas del efecto
        QJsonArray inputs = node_fx.value("inputs").toArray();

        // si esta vacia o no exite, crea una entrada
        if (inputs.isEmpty())
            inputs.push_back("Input-1");

        // sumamos la mascara, que index 0
        inputs.push_front("mask");
        bool has_mask = node_fx.value("mask").toBool();

        // Crea los links para el nodo
        links = new QList<node_link *>;
        for (int i = 0; i < inputs.count(); i++)
        {
            QString label = inputs[i].toString();

            node_link *link = new node_link(
                label, has_mask, i, props.scene, this, props.link_connecting,
                props.project, props.vinacomp, _node_graph);

            links->push_back(link);
            connected_indexs.push_back(false);
        }
        //

        this->setZValue((*props.current_z_value) + 1);

        _output_link = new output_link(props.scene, _node_view, this);
    }
}

node::~node()
{
    if (links)
    {
        for (node_link *link : *links)
            delete link;

        delete links;
    }
    delete _output_link;
    delete nodes_connected_to_the_inputs;
    delete nodes_connected_to_the_output;
    delete center_position;

    vinacomp *__vinacomp = static_cast<vinacomp *>(props.vinacomp);
    if (_trim_panel)
        delete _trim_panel;

    if (_viewer)
    {
        __vinacomp->get_viewers()->remove(_viewer->get_name());
        __vinacomp->get_panels_layout()->delete_viewer(_viewer);
        delete _viewer;
    }

    // falta borrar el panel de properties si es que esta
    // falta borrar del viewer de la lista de viewer de vinacomp
    // borrar viewer del panels_layout y de la lista del panel
}

void node::make_panel()
{
    QString name = get_name();

    // Crear panel de 'knobs'
    QStringList nodes_without_panel = {"viewer", "dot"};

    if (!nodes_without_panel.contains(type))
    {
        if (!_trim_panel)
            _trim_panel =
                new trim_panel(props._properties, name, type, color, icon_name,
                               nodes_loaded, props.project, props.vinacomp, this);
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

            __vinacomp->get_viewers()->insert(name, _viewer);
        }
        __vinacomp->get_panels_layout()->add_viewer(_viewer);
    }
    //
}

void node::refresh()
{
    if (!links)
        return;

    // Actualizacion de todos los links conectados al nodo
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

    _output_link->refresh();
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

    if (_output_link)
        _output_link->set_selected(enable);
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

QPointF node::get_center_position() const
{
    float x = this->x() + current_width / 2;
    float y = this->y() + current_height / 2;

    *center_position = {x, y};

    return *center_position;
}

node_link *node::get_close_link() const
{
    if (!_node_view)
        return nullptr;

    node_view *__node_view = static_cast<node_view *>(_node_view);

    int grip_distance = 150;
    node_link *close_link = nullptr;

    bool finded = false;
    for (node *_node : *__node_view->get_nodes())
    {
        auto *links = _node->get_links();
        if (!links)
            continue;

        for (node_link *link : *links)
        {
            if (!link->is_connected())
                continue;

            link->set_ghost_link(false);

            // si ya encontro el link cercano, no hace lo siguiente, asi
            // ahorramos recursos ya que no se puede hacer 'break' al for, ya
            // que tiene que ir apagando los link fantasmas que quedaron
            // prendidos
            if (finded)
                continue;

            QPointF center_link = link->get_center_position();
            float distance = qt::distance_points(*center_position, center_link);

            if (distance < grip_distance)
            {
                close_link = link;
                finded = true;
                break;
            }
        }
    }

    return close_link;
}

void node::insert_in_between()
{
    if (!_node_view)
        return;

    // tiene que ser antes que el qt::control para que se oculte el 'ghost link'
    node_link *close_link = get_close_link();
    if (!close_link)
        return;

    if (!qt::control() || !qt::shift() || !_node_view)
        return;

    close_link->set_ghost_link(false);
    close_link->insert_node_in_between(this);
}

void node::show_close_link()
{
    if (!_node_view)
        return;

    node_link *link_to_insert = get_close_link();
    if (!link_to_insert)
        return;

    if (!qt::control() || !qt::shift() || !_node_view)
        return;

    link_to_insert->set_ghost_link(true, *center_position);
}

node_link *node::get_link(int index) const
{
    if (!links)
        return nullptr;
    if (links->count() < 2)
        return nullptr;

    if (index == -1)
    {
        // si no tiene index como argumento, obtiene el primer index no
        // conectado, y si no hay ninguno no conectado, obtiene el index 1 que
        // el por defecto
        int _index = -1;
        for (bool connected : connected_indexs)
        {
            _index++;
            if (!connected && _index != 0)
                break;
        }

        if (_index == 0) // mask
            _index = 1;

        return links->value(_index);
    }
    return links->value(index);
}

QList<node_link *> node::get_output_links() const
{
    // obtiene todos los links conectados a este nodo
    QList<node_link *> links;

    for (node *out_node : *nodes_connected_to_the_output)
    {
        for (node_link *link : *out_node->get_links())
        {
            QGraphicsItem *connected_node = link->get_connected_node();
            if (connected_node == this)
                links.push_back(link);
        }
    }

    return links;
}

void node::snap_to_node(node *_node, QPointF this_node_pos, float &x_snap,
                        float &y_snap)
{
    if (selected_nodes->contains(_node->get_name()))
        return;

    int snap = 20;

    float size_x_difference =
        (_node->get_size().width() - this->get_size().width()) / 2;
    float size_y_difference =
        (_node->get_size().height() - this->get_size().height()) / 2;

    float _this_node_x = this_node_pos.x() - size_x_difference;
    float _this_node_y = this_node_pos.y() - size_y_difference;

    float x_difference = abs(_node->x() - _this_node_x);
    float y_difference = abs(_node->y() - _this_node_y);

    if (x_difference < snap)
        x_snap = _node->x() + size_x_difference;

    else if (y_difference < snap)
        y_snap = _node->y() + size_y_difference;

    // snap al nodo verticalmente
    int node_separation = 40;
    if (!y_snap)
    {
        float min_separation_y = (get_size().height() / 2) +
                                 (_node->get_size().height() / 2) +
                                 node_separation;

        y_difference = abs(min_separation_y - y_difference);

        if (y_difference < snap &&
            _node->get_center_position().x() == get_center_position().x())
        {
            if (this->y() > _node->y())
                y_snap =
                    _node->y() + _node->get_size().height() + node_separation;
            else
                y_snap =
                    _node->y() - this->get_size().height() - node_separation;
        }
    }
    //
    //

    // snap al nodo horizontalmente
    if (!x_snap)
    {
        float min_separation_x = (get_size().width() / 2) +
                                 (_node->get_size().width() / 2) +
                                 node_separation;

        x_difference = abs(min_separation_x - x_difference);

        if (x_difference < snap &&
            _node->get_center_position().y() == get_center_position().y())
        {
            if (this->x() > _node->x())
                x_snap =
                    _node->x() + _node->get_size().width() + node_separation;
            else
                x_snap =
                    _node->x() - this->get_size().width() - node_separation;
        }
    }
    //
    //
}
void node::set_center_position(float x, float y)
{
    set_position(x - (get_size().width() / 2), y - (get_size().height() / 2));
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

void node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    insert_in_between();
}

void node::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF position = mapToScene(event->pos());
    QPointF click_position_on_node = click_position - _freeze_position;

    float this_node_x = position.x() - click_position_on_node.x();
    float this_node_y = position.y() - click_position_on_node.y();

    float x_snap = 0;
    float y_snap = 0;

    // busca el snap en cada nodo conectado
    for (node *connected_node : *nodes_connected_to_the_inputs)
        snap_to_node(connected_node, {this_node_x, this_node_y}, x_snap,
                     y_snap);
    for (node *connected_node : *nodes_connected_to_the_output)
        snap_to_node(connected_node, {this_node_x, this_node_y}, x_snap,
                     y_snap);
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
    show_close_link();
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
