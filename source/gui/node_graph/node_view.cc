#include "node_backdrop.h"
#include "node_dot.h"
#include "node_group.h"
#include "node_rect.h"
#include <node_link.h>
#include <node_view.h>
#include <qt.h>
#include <util.h>
#include <vinacomp.h>
#include <maker.h>

// Engine
#include <project_struct.h>

node_view::node_view(QWidget *__vinacomp, properties *__properties,
                     nodes_load *_nodes_loaded, QWidget *__node_graph)

    : _properties(__properties)
    , _vinacomp(__vinacomp)
    , nodes_loaded(_nodes_loaded)
    , _node_graph(__node_graph)
    , output_link_node(nullptr)
    , insertion_between_nodes(true)
{

    scene = new QGraphicsScene();

    // cuadro de cambio de nombre de nodo
    node_rename_edit = new QLineEdit(this);
    connect(node_rename_edit, &QLineEdit::returnPressed, this,
            [this] { change_node_name(); });
    node_rename_edit->hide();
    //
    //

    this->setObjectName("node_view");
    this->setScene(scene);

    // desabilita el scroll
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //
    //

    // selection box
    selection_box = new QGraphicsRectItem();
    selection_box->setVisible(false);
    selection_box->setPen(QPen(QColor(0, 0, 0, 100), 0));
    selection_box->setBrush(QBrush(QColor(0, 0, 0, 30)));
    selection_box->setZValue(100000); // siempre por encima
    scene->addItem(selection_box);
    //
    //

    nodes = new QMap<QString, node *>;
    selected_nodes = new QMap<QString, node *>;
    link_connecting = new QJsonObject();

    current_z_value = new int();

    scene->setSceneRect(-500000, -500000, 1000000, 1000000);
    this->setRenderHint(QPainter::Antialiasing);

    setup_shortcut();
}

node_view::~node_view() {}

void node_view::setup_shortcut()
{
    qt::shortcut("Ctrl+A", this, [this]() { this->select_all(true); });

    qt::shortcut("N", this, [this]() { this->change_node_name_dialog(); });

    qt::shortcut("L", this, [this]() { this->align_selected_nodes(); });
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
    QList<node*> finded_nodes;
    for (node *_node : *nodes)
    {
        if (_node->get_name().toUpper().contains(key))
            select_node(_node->get_name(), true);
    }

    fit_view_to_nodes();
}

void node_view::connect_to_viewer()
{
    // conecta el nodo seleccionado al 'viewer', si no existe ningun 'viewer' en
    // el 'node_view', crea un nuevo 'viewer'
    node *_node = get_selected_node();
    if (!_node)
        return;

    node *_viewer = get_main_viewer();

    if (!_viewer)
    {
        maker *_maker = static_cast<node_graph *>(_node_graph)->get_maker();
        QString viewer_name = _maker->create_fx("viewer");
        _viewer = get_node(viewer_name);
    }

    // evita que se conecte asi mismo
    if (_viewer == _node)
        return;

    _viewer->get_link(1)->connect_node(_node);
}

node *node_view::get_main_viewer() const
{
    // obtiene el primer 'viewer' que encuentre
    for (node *_node : *nodes)
        if (_node->get_type() == "viewer")
            return _node;

    return nullptr;
}

QRectF node_view::bbox_nodes(QMap<QString, node *> *_nodes) const
{
    if (_nodes->empty())
        return {0, 0, 0, 0};

    // promedia la position de muchos nodos
    QPointF first_node_pos = _nodes->first()->pos();
    QSize first_node_size = _nodes->first()->get_size();

    float top = first_node_pos.y() + first_node_size.height();
    float bottom = first_node_pos.y();
    float left = first_node_pos.x();
    float right = first_node_pos.x() - first_node_size.width();

    for (node *_node : *_nodes)
    {
        QPointF position = _node->pos();
        QSize size = _node->get_size();
        int x2 = position.x() + size.width();
        int y2 = position.y() + size.height();

        // calcula el ancho y alto maximo de todos los nodos
        if (y2 > top)
            top = y2;
        if (position.y() < bottom)
            bottom = position.y();
        if (x2 > right)
            right = x2;
        if (position.x() < left)
            left = position.x();
        //
    }

    return {left, bottom, right - left, top - bottom};
}

void node_view::fit_view_to_nodes()
{
    float nodes_width = 0;
    float nodes_height = 0;

    auto get_pos = [&](QMap<QString, node *> *nodes) {
        if (nodes->empty())
            return QPointF{0, 0};

        QRectF bbox = bbox_nodes(nodes);
        float bottom = bbox.y();
        float top = bbox.y() + bbox.height();
        float left = bbox.x();
        float right = bbox.x() + bbox.width();

        nodes_width = abs(left - right);
        nodes_height = abs(bottom - top);

        float x = (left + right) / 2;
        float y = (bottom + top) / 2;

        x -= float(width()) / 2.0;
        y -= float(height()) / 2.0;

        return QPointF(x, y);
    };

    QPointF pos;
    if (selected_nodes->empty())
        pos = get_pos(nodes);
    else
        pos = get_pos(selected_nodes);

    float padding = 300;

    nodes_width += padding;
    nodes_height += padding;

    float _width = width();
    float _height = height();

    float aspect_x = 1;
    float aspect_y = 1;

    // si el ancho o el alto del conjunto de los nodos es mas grande que el
    // ancho o el alto del actual tamaño del viewer, crea un aspecto para luego
    // multiplicarlo por el ancho y alto.
    if (nodes_width > _width)
        aspect_x = nodes_width / _width;
    if (nodes_height > _height)
        aspect_y = nodes_height / _height;

    // deja el aspecto mas grande
    float aspect = aspect_x;
    if (aspect_y > aspect_x)
        aspect = aspect_y;

    _width *= aspect;
    _height *= aspect;

    // ajuste de position x e y, despues del cambio de escala ,
    // si la escala no cambia es 0
    float append_x = float(_width - width()) / 2.0;
    float append_y = float(_height - height()) / 2.0;

    this->fitInView(pos.x() - append_x, pos.y() - append_y, _width, _height);
    freeze_current_rect();
}

void node_view::align_selected_nodes()
{
    if (selected_nodes->empty())
        return;

    // obtiene el primer item seleccionado para sacar la posicion del nodo
    node *first_node = selected_nodes->first();
    //

    int separation = 20;
    int pos_x = first_node->x();
    int pos_y = first_node->y();
    for (node *selected_node : *selected_nodes)
    {
        selected_node->set_position(pos_x, pos_y);
        pos_x += separation + selected_node->get_size().width();
    }
}

void node_view::rename_node(node *_node, QString name, QString new_name)
{
    _node->set_name(new_name);

    nodes->remove(name);
    nodes->insert(new_name, _node);

    selected_nodes->remove(name);
    selected_nodes->insert(new_name, _node);

    _node->refresh();
}

void node_view::change_node_name_dialog()
{
    if (selected_nodes->empty())
        return;

    node *selected_node = selected_nodes->last();
    if (selected_node == NULL)
        return;

    node_rename_edit->setText(selected_node->get_name());
    QPoint position = this->mapFromGlobal(QCursor::pos());

    node_rename_edit->move(position);
    node_rename_edit->setFocus();
    node_rename_edit->show();
}

void node_view::change_node_name()
{
    // cambia el nombre al ultimo nodo seleccionado
    node *selected_node = selected_nodes->last();
    if (selected_node == NULL)
        return;

    QString old_name = selected_node->get_name();
    QString new_name = node_rename_edit->text();

    rename_node(selected_node, old_name, new_name);

    node_rename_edit->hide();
}

node *node_view::create_node(node_struct node_data, bool basic_creation,
                             bool from_project)
{
    node *selected_node = get_selected_node();

    // la 'basic_creation' : crea position bajo el cursor, no conecta el nodo y no
    // lo selecciona

    project_struct *project = static_cast<vinacomp *>(_vinacomp)->get_project();

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
        backdrop = new node_backdrop(props, selected_nodes, this);
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
        if (selected_node && !basic_creation)
        {
            position = {selected_node->x(), selected_node->y() + 90};
        }
        else
        {
            // crea el nodo en la posicion del puntero del mouse
            QPoint origin = this->mapFromGlobal(QCursor::pos());
            position = this->mapToScene(origin);
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

    // inserta un item de nodo en el proyecto, con los atributos necesarios para
    // renderizar el resto de atributos se generan al guardar el proyecto
    project->insert_node(node_data.name, node_data.type);
    //

    if (selected_node && !basic_creation)
    {
        if (!backdrop)
            _node->get_link(1)->connect_node(selected_node);
        select_node(selected_node->get_name(), false);
    }

    if (!basic_creation)
        select_node(node_data.name, true);

    return _node;
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

void node_view::connect_output_link(QPoint position_node)
{
    if (!output_link_node)
        return;

    node *node_output = output_link_node;
    output_link_node = nullptr;

    node *node_to_connect = get_node_from_position(position_node);
    if (!node_to_connect)
        return;

    node_to_connect->get_link()->connect_node(node_output);
}

node *node_view::get_node(QString name)
{
    return nodes->value(name);
}

void node_view::select_node(QString name, bool select)
{
    node *_node = get_node(name);
    if (!_node)
        return;

    _node->set_selected(select);

    if (select)
        selected_nodes->insert(name, _node);
    else
        selected_nodes->remove(name);
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

node_link *node_view::get_node_link(node *_node, int link_index)
{
    if (!_node)
        return NULL;

    node_link *link = _node->get_links()->value(link_index);

    return link;
}

void node_view::select_all(bool select)
{
    selected_nodes->clear();

    for (node *_node : *nodes)
        select_node(_node->get_name(), select);
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

void node_view::set_visible_ghost_dots(bool visible)
{
    // visibilidad para los ghost dots
    for (node *_node : *nodes)
    {
        auto *links = _node->get_links();
        if (!links)
            continue;

        for (node_link *link : *links)
            link->set_visible_ghost_dot(visible);
    }
}
