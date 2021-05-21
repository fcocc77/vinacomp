#include "node_group.h"
#include "node_rect.h"
#include <maker.h>
#include <node_link.h>
#include <node_view.h>
#include <qt.h>
#include <util.h>
#include <vinacomp.h>

// Engine
#include <project_struct.h>

node_view::node_view(QWidget *__vinacomp, properties *__properties,
                     nodes_load *_nodes_loaded, project_struct *_project,
                     QWidget *__node_graph)

    : _properties(__properties)
    , _vinacomp(__vinacomp)
    , nodes_loaded(_nodes_loaded)
    , _node_graph(__node_graph)
    , project(_project)
    , output_link_node(nullptr)
    , insertion_between_nodes(true)
    , expression_link_visible(true)
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
        _viewer = _maker->create_fx("viewer");
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

void node_view::change_node_name_dialog()
{
    if (selected_nodes->empty())
        return;

    node *selected_node = selected_nodes->last();
    if (!selected_node)
        return;

    node_rename_edit->setText(selected_node->get_label());
    QPoint position = this->mapFromGlobal(QCursor::pos());

    node_rename_edit->move(position);
    node_rename_edit->setFocus();
    node_rename_edit->show();
}

void node_view::change_node_name()
{
    // cambia el nombre al ultimo nodo seleccionado
    node *selected_node = selected_nodes->last();
    if (!selected_node)
        return;

    QString new_name = node_rename_edit->text();

    rename_node(selected_node, new_name);

    node_rename_edit->hide();
}

void node_view::switch_inputs_a_and_b()
{
    node_rect *selected_node = dynamic_cast<node_rect *>(get_selected_node());
    if (!selected_node)
        return;

    auto *links = selected_node->get_links();
    if (links->count() < 3)
        return;

    node_link *link_1 = links->value(1);
    node_link *link_2 = links->value(2);

    auto *node_1 = link_1->get_connected_node();
    auto *node_2 = link_2->get_connected_node();

    link_1->disconnect_node();
    link_2->disconnect_node();

    link_1->connect_node(node_2);
    link_2->connect_node(node_1);
}

QPointF node_view::get_min_node_separation(node *node_a, node *node_b) const
{
    QPointF center = node_b->get_center_position();
    int node_b_mid_height = node_b->get_size().height() / 2;
    int node_a_mid_height = node_a->get_size().height() / 2;

    int node_separation = 40;

    QPointF position = {center.x(), center.y() + node_a_mid_height +
                                        node_b_mid_height + node_separation};

    auto size = node_a->get_size();
    position = {position.x() - (size.width() / 2),
                position.y() - (size.height() / 2)};

    return position;
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

void node_view::center_node(node *_node)
{
    select_all(false);
    select_node(_node, true);
    fit_view_to_nodes();
}

void node_view::set_visible_expression_link(bool visible)
{
    expression_link_visible = visible;

    for (node *_node : *nodes)
        _node->set_visible_expression_link(visible);
}
