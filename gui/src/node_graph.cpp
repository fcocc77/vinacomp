#include <node_graph.hpp>

node_graph::node_graph(QJsonObject *_project)
{
    project = _project;
    scene = new QGraphicsScene();

    // cuadro de cambio de nombre de nodo
    node_rename_edit = new QLineEdit(this);
    connect(node_rename_edit, &QLineEdit::returnPressed, this, [this] {
        change_node_name();
    });
    node_rename_edit->hide();
    //
    //

    this->setObjectName("node_graph");
    this->setScene(scene);

    // desabilita el scroll
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //
    //

    // activacion de area de seleccion
    this->setDragMode(QGraphicsView::RubberBandDrag);
    //
    //

    nodes = new QMap<QString, node *>;
    nodes_links = new QMap<QString, QList<node_link *> *>;
    selected_nodes = new QMap<QString, node *>;
    link_connecting = new QJsonObject();

    current_z_value = new int();

    scene->setSceneRect(-500000, -500000, 1000000, 1000000);

    node *node1 = add_node("Shuffle", "shuffle_a", -100, 0,
                           "este es el tool tips\nOtra linea para probar");
    node *node2 = add_node("Transform_Mask", "transform_a", 100, 0, "tips otro");

    // node1->connect_input(0, node2);
    add_node("Grade", "grade_a", 300, 0);

    this->setRenderHint(QPainter::Antialiasing);

    setup_shortcut();
}

node_graph::~node_graph()
{
}

void node_graph::setup_shortcut()
{
    qt::shortcut("Ctrl+A", this, [this]() {
        this->select_all(true);
    });

    qt::shortcut("N", this, [this]() {
        this->change_node_name_dialog();
    });
}

void node_graph::change_node_name_dialog()
{
    node *selected_node = selected_nodes->last();
    if (selected_node == NULL)
        return;

    node_rename_edit->setText(selected_node->get_name());
    QPoint position = this->mapFromGlobal(QCursor::pos());

    node_rename_edit->move(position);
    node_rename_edit->setFocus();
    node_rename_edit->show();
}

void node_graph::change_node_name()
{
    // cambia el nombre al ultimo nodo seleccionado
    node *selected_node = selected_nodes->last();
    if (selected_node == NULL)
        return;

    QString old_name = selected_node->get_name();
    QString new_name = node_rename_edit->text();

    selected_node->set_name(new_name);

    nodes->remove(old_name);
    nodes->insert(new_name, selected_node);

    selected_nodes->remove(old_name);
    selected_nodes->insert(new_name, selected_node);

    auto node_links = nodes_links->value(old_name);
    nodes_links->remove(old_name);
    nodes_links->insert(new_name, node_links);

    node_rename_edit->hide();
}

node *node_graph::add_node(QString name, QString icon_name, int x, int y, QString tips)
{

    node *_node = new node(scene, current_z_value);
    _node->set_name(name);
    _node->set_position(x, y);
    _node->set_icon(icon_name);
    _node->set_tips(tips);

    // crea los links para el nodo
    int link_count = 1;
    QList<node_link *> *links = new QList<node_link *>;
    for (int i = 0; i < link_count; i++)
    {
        node_link *link = new node_link(i, scene, _node, link_connecting);
        links->push_back(link);
    }
    //
    //

    nodes->insert(name, _node);
    nodes_links->insert(name, links);

    return _node;
}

void node_graph::connect_node(QPoint position_node)
{
    // si un enlace input de un nodo esta siendo arrastrado para  conectarlo a otro nodo,
    // 'link_connecting' no estara vacio y se determinara
    // si se conecta o no al nodo de destino.
    if (!link_connecting->empty())
    {
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
    }
    //
    //
}

node *node_graph::get_node(QString name)
{
    return nodes->value(name);
}

void node_graph::select_node(QString name, bool select)
{
    node *_node = get_node(name);
    if (_node == NULL)
        return;

    _node->set_selected(select);

    for (node_link *link : *nodes_links->value(name))
        link->set_selected(select);

    if (select)
        selected_nodes->insert(name, _node);
    else
        selected_nodes->remove(name);
}

node *node_graph::get_node_from_position(QPoint position)
{
    // ya que el nodo esta compuesto por muchos hijos, al dar click puede ser un hijo,
    // y si es un hijo obtiene el nodo padre para poder extraer el nombre del nodo.
    QGraphicsItem *item = scene->itemAt(mapToScene(position), QTransform());
    if (!item)
        return NULL;

    QGraphicsItem *parent_item = item->parentItem();
    if (parent_item)
        item = parent_item;
    //
    //
    QString node_name = item->data(0).toString();

    return get_node(node_name);
}

node_link *node_graph::get_node_link(node *_node, int link_index)
{
    if (!_node)
        return NULL;

    auto links = nodes_links->value(_node->get_name());

    node_link *link = links->value(link_index);

    return link;
}

void node_graph::select_all(bool select)
{
    selected_nodes->clear();

    for (node *_node : *nodes)
        select_node(_node->get_name(), select);
}

void node_graph::select_nodes_by_area(QPointF selection_end_point)
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

    select_all(false);

    QList<QGraphicsItem *> selected_items = scene->items(rectangle);
    for (QGraphicsItem *item : selected_items)
    {
        QString node_name = item->data(0).toString();
        select_node(node_name, true);
    }
}

void node_graph::refresh_selected_nodes()
{
    auto refresh_node_link = [this](node *_node) {
        QList<node_link *> *links = nodes_links->value(_node->get_name());
        if (!links)
            return;

        for (node_link *_node_link : *links)
            _node_link->refresh();
    };

    // refresca los link de cada nodo seleccionado y los
    // link de los nodos que estan conectados a la salida.
    for (node *selected_node : *selected_nodes)
    {
        refresh_node_link(selected_node);
        for (node *output_node : *selected_node->get_output_nodes())
            refresh_node_link(output_node);
    }
    //
}