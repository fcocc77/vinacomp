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

    nodes = new QList<node *>;
    selected_nodes = new QList<node *>;
    current_z_value = new int();

    scene->setSceneRect(-500000, -500000, 1000000, 1000000);

    node *node1 = add_node("Shuffle", "shuffle_a", -100, 0,
                           "este es el tool tips\nOtra linea para probar");
    node *node2 = add_node("Transform Mask", "transform_a", 100, 0, "tips otro");

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