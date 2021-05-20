#include "node_backdrop.h"
#include "node_dot.h"
#include <knob_check_box.h>
#include <node_view.h>
#include <vinacomp.h>

node *node_view::create_node(node_struct node_data, bool basic_creation,
                             bool from_project)
{
    // la 'basic_creation' : crea position bajo el cursor, no conecta el nodo y
    // no lo selecciona

    // inserta un item de nodo en el proyecto, con los atributos necesarios para
    // renderizar el resto de atributos se generan al guardar el proyecto
    QJsonObject params;
    if (node_data.params)
        params = *node_data.params;

    project->insert_node(node_data.name, node_data.type, params);
    //

    node_props props;

    props.scene = scene;
    props.current_z_value = current_z_value;
    props.link_connecting = link_connecting;
    props.color = node_data.color;
    props.type = node_data.type;
    props.name = node_data.name;
    props.tips = node_data.tips;
    props.z_value = node_data.z_value;
    props.size = node_data.size;
    props._properties = _properties;
    props.vinacomp = _vinacomp;
    props.nodes_loaded = nodes_loaded;
    props.project = project;
    props.from_project = from_project;

    QPointF position = node_data.pos;
    QString type = node_data.type;

    // creacion de nodo segun clase
    node *_node;
    node_backdrop *backdrop = nullptr;
    if (type == "dot")
        _node = new node_dot(props, selected_nodes, _node_graph);
    else if (type == "backdrop")
    {
        backdrop = new node_backdrop(props, selected_nodes, _node_graph, this);
        _node = backdrop;
    }
    else if (type == "group")
        _node = new node_group(props, selected_nodes, _node_graph);
    else
        _node = new node_rect(props, selected_nodes, _node_graph);
    //
    //

    // si al posicion no viene del un proyecto
    if (!from_project)
    {
        node *selected_node = get_selected_node();

        if (selected_node && !basic_creation)
        {
            position = get_min_node_separation(_node, selected_node);
        }
        else
        {
            QWidget *over_widget = qApp->widgetAt(QCursor::pos());

            bool over_node_view = false;
            if (over_widget)
                over_node_view =
                    over_widget->parentWidget()->objectName() == "node_view";

            if (!over_node_view)
            {
                // posicion en el centro del 'node_view'
                position = mapToScene(QPoint{width() / 2, height() / 2});
            }
            else
            {
                // crea el nodo en la posicion del puntero del mouse
                QPoint origin = this->mapFromGlobal(QCursor::pos());
                position = this->mapToScene(origin);
            }

            auto size = _node->get_size();
            position = {position.x() - (size.width() / 2),
                        position.y() - (size.height() / 2)};
        }
    }
    //
    //

    if (backdrop)
    {
        if (!backdrop->is_selected_nodes())
            _node->set_position(position.x(), position.y());
    }
    else
    {
        _node->set_position(position.x(), position.y());
    }

    nodes->insert(node_data.name, _node);

    if (!basic_creation)
    {
        // conecta los nodos conectado al nodo seleccionado, al nuevo nodo
        if (!backdrop)
            connect_node_to_selected_nodes(_node);

        select_all(false);
        select_node(node_data.name, true);
    }

    static_cast<vinacomp *>(_vinacomp)->add_history();

    return _node;
}

void node_view::delete_selected_nodes()
{
    // crea una copia para que no de conflicto ya que en selected_nodes se
    // borraran items
    auto delete_nodes = *selected_nodes;

    for (node *node_to_delete : delete_nodes)
        delete_node(node_to_delete);
}

void node_view::delete_node(QString name)
{
    node *_node = get_node(name);
    if (!_node)
        return;

    delete_node(_node);

    static_cast<vinacomp *>(_vinacomp)->add_history();
}

void node_view::delete_node(node *_node)
{
    if (_node->get_type() != "backdrop")
        extract_node(_node);

    QString name = _node->get_name();

    _node->unlink_all();
    selected_nodes->remove(name);
    nodes->remove(name);
    copied_nodes.remove(name);
    project->delete_node(name);

    delete _node;
}

void node_view::extract_selected_nodes()
{
    QMap<node *, QList<node *>> extracted_nodes;

    // crea una lista con los nodos conectados, de cada nodo seleccionado
    for (node *selected_node : *selected_nodes)
    {
        if (selected_node->get_type() == "backdrop")
            continue;

        QList<node *> connecteds;
        for (node_link *link : *selected_node->get_links())
            connecteds.push_back(
                static_cast<node *>(link->get_connected_node()));

        extracted_nodes.insert(selected_node, connecteds);
    }

    // extrae todo los nodos seleccionados y los mueve hacia la derecha respecto
    // al bbox del conjunto
    QRectF bbox = bbox_nodes(selected_nodes);
    for (node *extracted_node : extracted_nodes.keys())
    {
        extract_node(extracted_node);
        extracted_node->set_position(extracted_node->x() + bbox.width(),
                                     extracted_node->y());
    }

    // conecta todos los nodos que se desconectaron al usar 'extract_node'
    for (auto *extracted_node : extracted_nodes.keys())
    {
        auto connected_nodes = extracted_nodes.value(extracted_node);

        for (int i = 0; i < extracted_node->get_links()->count(); i++)
        {
            node *connected_node = connected_nodes.value(i);
            if (!connected_node)
                continue;
            if (!selected_nodes->contains(connected_node->get_name()))
                continue;
            extracted_node->get_link(i)->connect_node(connected_node);
        }
    }
}

void node_view::extract_node(node *_node)
{
    // extrae los nodos de sus conecciones, y los nodos que estaban conectados,
    // los conecta a los nodos anterior y siguiente, esto sirve para el
    // 'delete_node' y 'cut_node'
    if (_node->get_type() == "backdrop")
        return;

    QGraphicsItem *node_from_input_1 = nullptr;
    node_link *link_1 = _node->get_link(1);
    if (link_1)
        node_from_input_1 = link_1->get_connected_node();

    for (node_link *link : *_node->get_links())
        link->disconnect_node();

    for (node_link *output_link : _node->get_output_links())
    {
        if (node_from_input_1)
            output_link->connect_node(node_from_input_1);
        else
            output_link->disconnect_node();
    }
}

void node_view::copy_nodes()
{
    copied_nodes = *selected_nodes;
}

void node_view::paste_nodes()
{
    auto get_basename = [](QString name) {
        QString str;
        for (QChar letter : name)
            if (!letter.isDigit())
                str += letter;
        return str;
    };

    auto get_available_name = [=](QString copied_name) {
        QString basename = get_basename(copied_name);
        QString available_name;

        int number = 1;
        while (true)
        {
            available_name = basename + QString::number(number);
            if (!get_node(available_name))
                break;
            number++;
        }
        return available_name;
    };

    QPointF cursor_pos = this->mapToScene(this->mapFromGlobal(QCursor::pos()));

    QRectF bbox = bbox_nodes(&copied_nodes);
    float center_bbox_x = (bbox.x() + (bbox.x() + bbox.width())) / 2;
    float center_bbox_y = (bbox.y() + (bbox.y() + bbox.height())) / 2;

    QMap<node*, node*> map_copies_nodes;

    for (node *copied_node : copied_nodes)
    {
        node_struct params;
        params.color = copied_node->get_color();
        params.pos = copied_node->pos();
        params.type = copied_node->get_type();
        params.inputs = {};
        params.tips = copied_node->get_tips();
        params.size = copied_node->get_size();
        params.name = get_available_name(copied_node->get_name());

        // aqui no se esta clonando el espacio de memoria 'params'
        // ya que cuando se cree el nodo en el proyecto 'insert_node' creara un
        // 'params' nuevo y copiara los datos
        params.params = copied_node->get_params();

        node *pasted_node = create_node(params, true, true);
        QPointF copied_center = copied_node->get_center_position();
        float x = cursor_pos.x() + (copied_center.x() - center_bbox_x);
        float y = cursor_pos.y() + (copied_center.y() - center_bbox_y);
        pasted_node->set_center_position(x, y);

        map_copies_nodes.insert(copied_node, pasted_node);
    }

    select_all(false);

    // conecta los nodos pegados, a partir de los nodos copiados
    for (node *copied_node : map_copies_nodes.keys())
    {
        node *pasted_node = map_copies_nodes.value(copied_node);
        select_node(pasted_node, true);

        node_backdrop *pasted_backdrop =
            dynamic_cast<node_backdrop *>(pasted_node);

        if (pasted_backdrop)
        {
            node_backdrop *copied_backdrop =
                dynamic_cast<node_backdrop *>(copied_node);

            pasted_backdrop->set_size(copied_node->get_size().width(),
                                      copied_node->get_size().height());

            pasted_backdrop->set_z_value(copied_backdrop->get_z_value());
            pasted_backdrop->refresh();
            continue;
        }

        for (int i = 0; i < copied_node->get_links()->count(); i++)
        {
            node_link *copied_link = copied_node->get_link(i);
            node_link *pasted_link = pasted_node->get_link(i);

            node *connected_node_from_copied =
                static_cast<node *>(copied_link->get_connected_node());

            if (connected_node_from_copied)
            {
                node *connected_node_from_pasted =
                    map_copies_nodes.value(connected_node_from_copied);

                pasted_link->connect_node(connected_node_from_pasted);
            }
        }
    }
}

node *node_view::get_node(QString name)
{
    return nodes->value(name);
}

bool node_view::rename_node(node *_node, QString new_name)
{
    QString aux = new_name;
    if (get_node(new_name))
    {
        QString msg =
            "The name: '" + new_name + "' already exists in another node";
        QMessageBox::warning(this, "VinaComp Rename", msg, QMessageBox::Ok);
        return false;
    }
    // no permite espacios vacios
    else if (aux.replace(" ", "").isEmpty())
        return false;

    QString name = _node->get_name();
    _node->set_name(new_name);

    project->rename_node(name, new_name);

    nodes->remove(name);
    nodes->insert(new_name, _node);

    selected_nodes->remove(name);
    selected_nodes->insert(new_name, _node);

    _node->refresh();

    return true;
}

node *node_view::get_selected_node() const
{
    // obtiene 1 solo nodo seleccionado, si hay mas de 1 nodo seleccionado
    // retorna nulo y si no hay nodos seleccionado retorna nulo tambien
    if (selected_nodes->count() != 1)
        return nullptr;

    node *_node = selected_nodes->first();
    return _node;
}

void node_view::select_node(node *_node, bool select)
{
    if (!_node)
        return;

    _node->set_selected(select);

    if (select)
        selected_nodes->insert(_node->get_name(), _node);
    else
        selected_nodes->remove(_node->get_name());
}

void node_view::select_node(QString name, bool select)
{
    select_node(get_node(name), select);
}

void node_view::select_all(bool select)
{
    selected_nodes->clear();

    for (node *_node : *nodes)
        select_node(_node, select);
}

void node_view::connect_node_to_selected_nodes(node *_node)
{
    if (selected_nodes->empty())
        return;

    auto *links = _node->get_links();

    // crea una lista con los link y nodos seleccionados que se van a conectar
    QList<std::pair<node_link *, node *>> items_to_connect;
    for (int i = 1; i < links->count(); i++)
    {
        node_link *link = links->value(i);

        node *selected_node = selected_nodes->values().value(i - 1);
        if (!selected_node)
            break;

        items_to_connect.push_back({link, selected_node});
    }
    //
    //

    auto connect_link = [=](node_link *link, node *to_node) {
        link->connect_node(to_node);

        // conecta los nodos conectado al nodo seleccionado, al nuevo nodo
        for (node_link *sel_link : to_node->get_output_links())
            sel_link->connect_node(_node);
    };

    int count = items_to_connect.count();

    // si hay 2 nodos que se pueden conectar, deja el nodo esquinado entre los 2
    // y conecta el nodo de mas arriba al link 1
    if (count == 2)
    {
        node *node_a = items_to_connect.first().second;
        node *node_b = items_to_connect.last().second;

        QPointF position;
        if (node_a->y() < node_b->y())
        {
            connect_link(links->value(1), node_a);
            connect_link(links->value(2), node_b);
            position = {node_a->get_center_position().x(),
                        node_b->get_center_position().y()};
        }
        else
        {
            connect_link(links->value(1), node_b);
            connect_link(links->value(2), node_a);
            position = {node_b->get_center_position().x(),
                        node_a->get_center_position().y()};
        }

        _node->set_center_position(position.x(), position.y());
    }
    else
    {
        for (auto item : items_to_connect)
            connect_link(item.first, item.second);

        // si los nodos seleccionados no son 2, deja el primer nodo bajo el otro
        node *first_node = items_to_connect.first().second;
        QPointF position = get_min_node_separation(_node, first_node);
        _node->set_position(position.x(), position.y());
    }
}

void node_view::find_nodes(QString key)
{
    // encuentra los nodos a partir de una key los selecciona
    // y los ajusta al 'node_view'
    select_all(false);
    if (key.isEmpty())
    {
        fit_view_to_nodes();
        return;
    }

    key = key.toUpper();
    QList<node *> finded_nodes;
    for (node *_node : *nodes)
    {
        if (_node->get_name().toUpper().contains(key))
            select_node(_node, true);
    }

    fit_view_to_nodes();
}

void node_view::select_connected_nodes(node *root_node)
{
    select_node(root_node, true);

    for (node *_node : *root_node->get_input_nodes())
        select_connected_nodes(_node);
}

void node_view::disable_selected_nodes()
{
    if (selected_nodes->empty())
        return;

    bool disable = false;
    bool keep_disable = false;
    for (node *_node : *selected_nodes)
    {
        node_rect *_node_rect = dynamic_cast<node_rect *>(_node);
        if (!_node_rect)
            continue;

        if (_node_rect->get_type() == "viewer")
            continue;

        if (!keep_disable)
        {
            disable = _node_rect->is_disable();
            keep_disable = true;
        }

        _node_rect->set_disable(!disable);

        // desabilita el nodo en el proyecto
        auto *params = _node->get_params();
        if (!disable)
            params->insert("disable_node", true);
        else
        {
            params->remove("disable_node");
            params->remove("disable_node_anim");
        }
        //

        trim_panel *panel = _node->get_trim_panel();
        if (panel)
        {
            knob_check_box *disable_node_knob =
                static_cast<knob_check_box *>(panel->get_knob("disable_node"));
            disable_node_knob->set_check(!disable, false);
        }
    }
    static_cast<vinacomp *>(_vinacomp)->update_render_all_viewer(true);
}

node_link *node_view::get_node_link(node *_node, int link_index)
{
    if (!_node)
        return NULL;

    node_link *link = _node->get_links()->value(link_index);

    return link;
}

node *node_view::get_node_from_position(QPoint position)
{
    // ya que el nodo esta compuesto por muchos hijos, al dar click puede ser un
    // hijo, y si es un hijo obtiene el nodo padre para poder extraer el nombre
    // del nodo.
    QGraphicsItem *item = scene->itemAt(mapToScene(position), QTransform());
    if (!item)
        return nullptr;

    QGraphicsItem *parent_item = item->parentItem();
    if (parent_item)
        item = parent_item;
    //
    //
    QString node_name = item->data(0).toString();

    return get_node(node_name);
}

void node_view::connect_node(QPoint position_node)
{
    // si un enlace input de un nodo esta siendo arrastrado para conectarlo a
    // otro nodo, 'link_connecting' no estara vacio y se determinara si se
    // conecta o no al nodo de destino.
    if (link_connecting->empty())
        return;

    QString node_name = link_connecting->value("name").toString();
    int link_index = link_connecting->value("index").toInt();

    node *from_node = get_node(node_name);
    node_link *link = get_node_link(from_node, link_index);

    node *to_node = get_node_from_position(position_node);
    if (!to_node)
        link->disconnect_node();
    else
    {
        // evita que se conecte asi mismo
        if (from_node != to_node)
            link->connect_node(to_node);
        else
            link->disconnect_node();
    }
    *link_connecting = {};
    //
    //
}

void node_view::select_nodes_by_area(QPointF selection_end_point)
{
    QPointF selection_start_point = mapToScene(click_position);

    QPainterPath rectangle;

    int start_x = selection_start_point.x();
    int start_y = selection_start_point.y();

    int end_x = selection_end_point.x();
    int end_y = selection_end_point.y();

    int width = end_x - start_x;
    int height = end_y - start_y;

    QRectF rect(start_x, start_y, width, height);
    rectangle.addRect(rect);

    // el 'QGraphicsRectItem' que se usa para el 'selection_box' da conflicto si
    // es negativo el ancho y alto, si es negativo lo invierte y se lo resta a
    // la position.
    if (width < 0)
    {
        width = -width;
        start_x -= width;
    }
    if (height < 0)
    {
        height = -height;
        start_y -= height;
    }
    QRectF rect_box(start_x, start_y, width, height);
    selection_box->setRect(rect_box);
    selection_box->setVisible(true);
    //

    if (!qt::shift())
        select_all(false);

    QList<QGraphicsItem *> selected_items = scene->items(rectangle);
    for (QGraphicsItem *item : selected_items)
    {
        node_backdrop *backdrop = dynamic_cast<node_backdrop *>(item);
        if (backdrop)
            if (!backdrop->is_under_selector(rect_box))
                continue;

        QString node_name = item->data(0).toString();
        select_node(node_name, true);
    }
}
