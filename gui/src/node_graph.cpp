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

    qt::shortcut("G", this, [this]() {
        for (int i = 0; i < 1000; i++)
        {
            QString name = "Grade" + QString::number(i);
            if (!get_node(name))
            {
                create_node(name, "grade_a", QPointF(0, 0));
                break;
            }
        }
    });
}

void node_graph::change_node_name_dialog()
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

node *node_graph::create_node(QString name, QString icon_name, QPointF position, QString tips)
{
    // QPointF position = mapToScene(QCursor::pos());

    node *_node = new node(scene, current_z_value);
    _node->set_name(name);
    _node->set_position(position.x(), position.y());
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

    if (!qt::shift())
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

QJsonObject node_graph::get_tree()
{
    // genera un arbol con todos los nodos con su informacion,
    // para guardarla en el proyecto.
    QJsonObject tree = {};

    for (node *_node : *nodes)
    {
        auto *links = nodes_links->value(_node->get_name());
        QJsonArray inputs = {};

        for (node_link *link : *links)
        {
            node *connected_node = link->get_connected_node();
            QString _connected_node = "NULL";
            if (connected_node)
                _connected_node = connected_node->get_name();

            QJsonObject link_data = {
                {"connected_node", _connected_node}};

            inputs.push_back(link_data);
        }

        QJsonObject data = {
            {"position", QJsonArray{_node->x(), _node->y()}},
            {"color", QJsonArray{30, 30, 30}},
            {"inputs", inputs}};

        tree[_node->get_name()] = data;
    }

    return tree;
}

void node_graph::restore_tree(QJsonObject nodes)
{

    for (QString name : nodes.keys())
    {
        QJsonObject data = nodes.value(name).toObject();

        QPointF position = {data["position"].toArray()[0].toDouble(),
                            data["position"].toArray()[1].toDouble()};
        create_node(
            name,
            "grade_a",
            position);
    }

    // conecta todos los nodos
    for (QString name : nodes.keys())
    {
        QJsonObject data = nodes.value(name).toObject();
        auto *links = nodes_links->value(name);

        QJsonArray inputs = data["inputs"].toArray();

        for (int i = 0; i < inputs.count(); i++)
        {
            QJsonObject link_data = inputs[i].toObject();
            QString connected_node = link_data["connected_node"].toString();
            node *node_to_connect = get_node(connected_node);

            node_link *link = links->value(i);
            if (link)
                link->connect_node(node_to_connect);
        }
    }

    //
}

QJsonObject node_graph::get_scene_data()
{
    QJsonObject matrix = {
        {"m11", this->transform().m11()},
        {"m12", this->transform().m12()},
        {"m13", this->transform().m13()},
        {"m21", this->transform().m21()},
        {"m22", this->transform().m22()},
        {"m23", this->transform().m23()},
        {"m31", this->transform().m31()},
        {"m32", this->transform().m32()},
        {"m33", this->transform().m33()}};

    QJsonObject scene_data = {};

    scene_data["matrix"] = matrix;

    return scene_data;
}

void node_graph::restore_scene_data(QJsonObject scene_data)
{
    QJsonObject matrix = scene_data["matrix"].toObject();

    QTransform _transform(
        matrix["m11"].toDouble(),
        matrix["m12"].toDouble(),
        matrix["m13"].toDouble(),
        matrix["m21"].toDouble(),
        matrix["m22"].toDouble(),
        matrix["m23"].toDouble(),
        matrix["m31"].toDouble(),
        matrix["m32"].toDouble(),
        matrix["m33"].toDouble());

    this->setTransform(_transform);
}