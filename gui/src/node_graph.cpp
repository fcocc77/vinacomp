#include <node_graph.hpp>

node_graph::node_graph(QJsonObject *_project)
{
    project = _project;
    scene = new QGraphicsScene();
    node_rename_edit = new QLineEdit(this);

    this->setObjectName("node_graph");
    this->setScene(scene);

    // desabilita el scroll
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //
    //

    nodes = new QList<node *>;
    selected_nodes = new QList<node *>;

    scene->setSceneRect(-500000, -500000, 1000000, 1000000);

    node *node1 = add_node("Shuffle", "shuffle_a", -100, 0);
    node *node2 = add_node("Transform Mask", "transform_a", 100, 0);

    node1->connect_input(0, node2);
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
    });

    qt::shortcut("Ctrl+A", this, [this]() {
        this->select_all(true);
    });

    qt::shortcut("N", this, [this]() {
        this->change_node_name();
    });
}

void node_graph::change_node_name()
{
    // cambia el nombre al ultimo nodo seleccionado
    node *selected_node = selected_nodes->last();

    if (selected_node == NULL)
        return;

    QString name = "Nodos";

    node_rename_edit->setText("hola");

    // get the position relative to the scene
    QPointF sp = selected_node->scenePos();
    // or use
    sp = selected_node->mapToScene(selected_node->pos());

    // find the global (screen) position of the item
    QPoint global = this->mapToGlobal(this->mapFromScene(sp));

    node_rename_edit->move(sp.x(), sp.y());
    node_rename_edit->show();
    // selected_node->set_name(name);

    print(selected_node);
}

node *node_graph::add_node(QString name, QString icon_name, int x, int y)
{

    node *_node = new node(scene, selected_nodes);
    _node->set_name(name);
    _node->set_position(x, y);
    _node->set_icon(icon_name);

    nodes->push_back(_node);

    return _node;
}

void node_graph::select_all(bool select)
{
    selected_nodes->clear();

    for (node *_node : *nodes)
        _node->select(select);
}

void node_graph::refresh_selected_nodes()
{
    for (node *_node : *selected_nodes)
        _node->refresh();
}