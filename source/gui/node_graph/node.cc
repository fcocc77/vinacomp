#include "./node.h"
#include <panels_layout.h>
#include <vinacomp.h>
#include <util.h>
#include <node_graph.h>
#include <node_view.h>

node::node(node_props _props, QMap<QString, node *> *_selected_nodes,
           QWidget *_node_graph)

    : _trim_panel(nullptr)
    , _node_view(nullptr)
    , nodes_connected_to_the_inputs(new QMap<QString, node *>)
    , nodes_connected_to_the_output(new QMap<QString, node *>)
    , selected_nodes(_selected_nodes)
    , inputs(nullptr)
    , selected(false)
    , is_backdrop(_props.type == "backdrop")
    , _output_wire(nullptr)
    , links(nullptr)
    , center_position(new QPointF)
    , nodes_loaded(_props.nodes_loaded)
    , props(_props)

{
    if (_node_graph)
        _node_view = static_cast<node_graph *>(_node_graph)->get_node_view();

    name = props.name;
    tips = props.tips;
    type = props.type;
    label = name.split('.').last();

    if (name.contains('.'))
        // si el nombre tiene un punto, significa que esta dentro de un grupo
        group_name = name.left(name.lastIndexOf('.'));

    set_color(props.color);

    props.scene->addItem(this);

    if (is_backdrop)
        return;

    this->setZValue((*props.current_z_value) + 1);

    if (type != "input")
    {
        QJsonObject node_fx = nodes_loaded->get_effect(props.type);
        // carga las entradas del efecto
        QJsonArray _inputs = node_fx.value("inputs").toArray();

        // si esta vacia o no exite, crea una entrada
        if (_inputs.isEmpty())
            _inputs.push_back("Input-1");

        // sumamos la mascara, que index 0
        _inputs.push_front("mask");
        bool has_mask = node_fx.value("mask").toBool();

        // Crea los inputs para el nodo
        inputs = new QList<input_wire *>;
        for (int i = 0; i < _inputs.count(); i++)
        {
            QString label = _inputs[i].toString();

            input_wire *input = new input_wire(
                label, has_mask, i, props.scene, this, props.input_connecting,
                props.project, props.vinacomp, _node_graph);

            inputs->push_back(input);
            connected_indexs.push_back(false);
        }
    }

    links = new node_links(props.scene, _node_view, this);

    if (type != "output")
        _output_wire = new output_wire(props.scene, _node_view, this);
}

node::~node()
{
    if (inputs)
    {
        for (input_wire *input : *inputs)
            delete input;

        delete inputs;
    }
    delete _output_wire;
    delete links;
    delete nodes_connected_to_the_inputs;
    delete nodes_connected_to_the_output;
    delete center_position;

    if (_trim_panel)
        delete _trim_panel;

    // falta borrar el panel de properties si es que esta
}

void node::make_panel(bool float_panel)
{
    QString name = get_name();

    // Crear panel de 'knobs'
    QStringList nodes_without_panel = {"viewer", "dot", "input", "output"};

    if (!nodes_without_panel.contains(type))
    {
        if (!_trim_panel)
            _trim_panel = new trim_panel(props._properties, name, type, color,
                                         icon_name, nodes_loaded, props.project,
                                         props.vinacomp, this, _node_view);
        if (float_panel)
            _trim_panel->float_panel(true);
        else
            props._properties->add_trim_panel(_trim_panel);
    }
}

void node::refresh()
{
    if (is_backdrop)
        return;

    // Actualizacion de todos los inputs conectados al nodo
    auto refresh_wires = [](node *_node) {
        auto *_inputs = _node->get_inputs();
        if (_inputs)
            for (input_wire *_input_wire : *_inputs)
                _input_wire->refresh();

        if (_node->get_output_wire())
            _node->get_output_wire()->refresh();
    };

    refresh_wires(this);

    // refresca los input de cada nodo seleccionado y los
    // input de los nodos que estan conectados a la salida.
    for (node *output_node : *this->get_output_nodes())
        if (output_node)
            refresh_wires(output_node);

    for (node *slave_node : slaves_nodes)
        slave_node->get_expression_link()->refresh();

    links->refresh();
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

    if (inputs)
        for (input_wire *input : *inputs)
            input->set_selected(enable);

    if (_output_wire)
        _output_wire->set_selected(enable);
}

void node::set_name(QString _name)
{
    this->setData(0, _name);

    name = _name;
    label = name.split('.').last();

    if (_trim_panel)
        _trim_panel->set_name(_name);
}

void node::rename(QString new_name)
{
    // crea lista auxiliar de los nodos conectados, desconecta todos los input y
    // los conecta despues de cambiar el nombre de este nodo
    QList<QGraphicsItem *> aux_connected_nodes;
    if (inputs)
    {
        for (input_wire *input : *inputs)
        {
            aux_connected_nodes.push_back(input->get_connected_node());
            input->disconnect_node();
        }
    }

    auto aux_output_wires = get_inputs_connected_to_this();
    for (input_wire *output_wire : aux_output_wires)
        output_wire->disconnect_node();
    //
    //

    QString old_name = get_name();

    props.project->rename_node(old_name, new_name, false);
    set_name(new_name);

    // restaura los link de los 'slaves_nodes' en los nodos manejadores
    for (node *handler_node : handler_nodes)
    {
        trim_panel *handler_panel = handler_node->get_trim_panel();
        if (handler_panel)
            for (knob *handler_knob : *handler_panel->get_knobs())
                handler_knob->restore_slaves_konbs();
    }

    // renombra el nodo handler en todos los esclavos
    for (node *slave_node : slaves_nodes)
    {
        trim_panel *slave_panel = slave_node->get_trim_panel();
        if (slave_panel)
            for (knob *slave_knob : *slave_panel->get_knobs())
                slave_knob->rename_handler_node_name(old_name, new_name);
    }

    // vuelve a conectar los nodos
    if (inputs)
    {
        for (int i = 0; i < aux_connected_nodes.count(); i++)
        {
            auto *_node = aux_connected_nodes.at(i);
            inputs->at(i)->connect_node(_node);
        }
    }

    for (input_wire *output_wire : aux_output_wires)
        output_wire->connect_node(this);
}

void node::set_group_name(QString _group_name)
{
    group_name = _group_name;
    static_cast<node_view *>(_node_view)->rename_node(this, label);
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

input_wire *node::get_close_input() const
{
    if (!_node_view)
        return nullptr;

    node_view *__node_view = static_cast<node_view *>(_node_view);

    int grip_distance = 150;
    input_wire *close_input = nullptr;

    bool finded = false;
    for (node *_node : *__node_view->get_nodes())
    {
        auto *_inputs = _node->get_inputs();
        if (!_inputs)
            continue;

        for (input_wire *input : *_inputs)
        {
            if (!input->is_connected())
                continue;

            input->set_ghost_wire(false);

            // si ya encontro el input cercano, no hace lo siguiente, asi
            // ahorramos recursos ya que no se puede hacer 'break' al for, ya
            // que tiene que ir apagando los input fantasmas que quedaron
            // prendidos
            if (finded)
                continue;

            QPointF center_input = input->get_center_position();
            float distance = qt::distance_points(*center_position, center_input);

            if (distance < grip_distance)
            {
                close_input = input;
                finded = true;
                break;
            }
        }
    }

    return close_input;
}

void node::insert_in_between()
{
    if (!_node_view)
        return;

    // tiene que ser antes que el qt::control para que se oculte el 'ghost input'
    input_wire *close_input = get_close_input();
    if (!close_input)
        return;

    if (!qt::control() || !qt::shift() || !_node_view)
        return;

    close_input->set_ghost_wire(false);
    close_input->insert_node_in_between(this);
}

void node::show_close_input_wire()
{
    if (!_node_view)
        return;

    input_wire *input_to_insert = get_close_input();
    if (!input_to_insert)
        return;

    if (!qt::control() || !qt::shift() || !_node_view)
        return;

    input_to_insert->set_ghost_wire(true, *center_position);
}

input_wire *node::get_input(int index) const
{
    if (!inputs)
        return nullptr;
    if (inputs->count() < 2)
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

        return inputs->value(_index);
    }
    return inputs->value(index);
}

QList<input_wire *> node::get_inputs_connected_to_this() const
{
    // obtiene todos los inputs conectados a este nodo
    QList<input_wire *> inputs;

    for (node *out_node : *nodes_connected_to_the_output)
    {
        for (input_wire *input : *out_node->get_inputs())
        {
            QGraphicsItem *connected_node = input->get_connected_node();
            if (connected_node == this)
                inputs.push_back(input);
        }
    }

    return inputs;
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

void node::add_handler_node(node *new_handler_node)
{
    if (!new_handler_node)
        return;

    if (handler_nodes.contains(new_handler_node))
        return;

    handler_nodes.push_back(new_handler_node);
    new_handler_node->add_slave_node(this);

    links->enable_links(handler_nodes.count());

    if (static_cast<node_view *>(_node_view)->are_visible_links())
        links->set_visible(true);

    links->refresh();
}

void node::remove_handler_node(node *handler_node)
{
    handler_node->remove_slave_node(this);
    handler_nodes.removeOne(handler_node);

    links->enable_links(handler_nodes.count());
    links->refresh();
}

void node::add_handler_node(QString node_name)
{
    if (node_name.isEmpty())
        return;

    add_handler_node(
        static_cast<node_view *>(_node_view)->get_node(node_name, true));
}

void node::remove_handler_node(QString node_name)
{
    node *handler_node =
        static_cast<node_view *>(_node_view)->get_node(node_name, true);

    if (!handler_node)
        return;

    remove_handler_node(handler_node);
}

void node::unlink_all()
{
    for (node *slave_node : slaves_nodes)
    {
        if (slave_node->get_trim_panel())
            slave_node->get_trim_panel()->unlink_node_to_knobs(
                this->get_name());

        slave_node->remove_handler_node(this);
    }

    if (this->get_trim_panel())
        this->get_trim_panel()->unlink_all_knobs();
    else
        for (node *handler_node : this->get_handler_nodes())
            this->remove_handler_node(handler_node);

    props.project->unlink_all_knobs(get_name());
}

void node::unlink_all_invisible_node(QWidget *_vinacomp, QString node_name)
{
    // es lo mismo que 'unlink_all' pero este es un metodo estatico
    // que desvincula todos los nodos 'handler' y 'slave' que estan creados,
    // desde un nodos que no esta creado, sacando la informacion del proyecto

    vinacomp *vina = static_cast<vinacomp *>(_vinacomp);
    project_struct *project = vina->get_project();

    node_view *main_node_view = vina->get_main_node_graph()->get_node_view();

    node_struct &_node = project->nodes[node_name];

    for (QString slave_node_name : project->get_slaves_nodes(&_node))
    {
        node *slave_node = main_node_view->get_node(slave_node_name, true);
        if (!slave_node)
            continue;

        if (slave_node->get_trim_panel())
            slave_node->get_trim_panel()->unlink_node_to_knobs(node_name);
    }

    project->unlink_all_knobs(node_name);
}

void node::add_slave_node(node *_node)
{
    if (!slaves_nodes.contains(_node))
        slaves_nodes.push_back(_node);
}

void node::remove_slave_node(node *_node)
{
    slaves_nodes.removeOne(_node);
}

void node::set_visible_link(bool visible)
{
    if (links)
        links->set_visible(visible);
}

void node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    make_panel(qt::control());
}

void node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (!is_backdrop)
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
    show_close_input_wire();
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
