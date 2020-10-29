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

    nodes = new QMap<QString, node *>;
    nodes_links = new QMap<QString, QList<node_link *> *>;
    selected_nodes = new QMap<QString, node *>;
    selected_nodes_links = new QMap<QString, QList<node_link *> *>;

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
    qt::shortcut("Escape", this, [this]() {
        this->select_all(false);
        node_rename_edit->hide();
    });

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

    selected_node->set_name(node_rename_edit->text());
    node_rename_edit->hide();
}

node *node_graph::add_node(QString name, QString icon_name, int x, int y, QString tips)
{

    node *_node = new node(scene, selected_nodes, current_z_value);
    _node->set_name(name);
    _node->set_position(x, y);
    _node->set_icon(icon_name);
    _node->set_tips(tips);

    // crea los links para el nodo
    int link_count = 1;
    QList<node_link *> *links = new QList<node_link *>;
    for (int i = 0; i < link_count; i++)
    {
        node_link *link = new node_link(scene, _node);
        links->push_back(link);
    }
    //
    //

    nodes->insert(name, _node);
    nodes_links->insert(name, links);

    return _node;
}

void node_graph::select_node(QString name, bool select)
{
    node *_node = nodes->value(name);
    if (_node == NULL)
        return;

    _node->set_selected_style(select);

    if (select)
        selected_nodes->insert(name, _node);
    else
        selected_nodes->remove(name);
}

QString node_graph::get_node_name_from_position(QPoint position)
{
    // ya que el nodo esta compuesto por muchos hijos, al dar click puede ser un hijo,
    // y si es un hijo obtiene el nodo padre para poder extraer el nombre del nodo.
    QGraphicsItem *item = scene->itemAt(mapToScene(position), QTransform());
    if (item == NULL)
        return "";

    QGraphicsItem *parent_item = item->parentItem();
    if (parent_item != NULL)
        item = parent_item;
    //
    //
    return item->data(0).toString();
}

void node_graph::select_all(bool select)
{
    selected_nodes->clear();

    for (node *_node : *nodes)
        select_node(_node->get_name(), select);
}

void node_graph::refresh_selected_nodes()
{

    // refresca los link de cada nodo seleccionado
    for (node *_node : *selected_nodes)
    {
        QList<node_link *> *links = selected_nodes_links->value(_node->get_name());

        if (links == NULL)
            continue;

        print(*links);
        // for (node_link *_node_link : *links)
        //     _node_link->refresh();
    }

    //
}