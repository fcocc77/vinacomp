#include <node_graph.hpp>

node_graph::node_graph(/* args */)
{
    scene = new QGraphicsScene();

    this->setObjectName("node_graph");
    this->setScene(scene);

    // desabilita el scroll
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //
    //

    nodes = new QList<node *>;

    scene->setSceneRect(-500000, -500000, 1000000, 1000000);

    panning = false;
    pressed = false;

    add_node("Crop", -100, 0);
    add_node("Transform", 100, 0);
    add_node("Grade", 300, 0);

    this->setRenderHint(QPainter::Antialiasing);

    setup_shortcut();
}

node_graph::~node_graph()
{
}

void node_graph::setup_shortcut()
{
    QShortcut *deselect_all_shortcut = new QShortcut(QKeySequence("Escape"), this);
    QObject::connect(deselect_all_shortcut, &QShortcut::activated, this, [this]() {
        this->select_all(false);
    });

    QShortcut *select_all_shortcut = new QShortcut(QKeySequence("Ctrl+A"), this);
    QObject::connect(select_all_shortcut, &QShortcut::activated, this, [this]() {
        this->select_all(true);
    });
}

void node_graph::add_node(QString name, int x, int y)
{

    node *_node = new node(scene, nodes);
    _node->set_name(name);
    _node->set_position(x, y);

    nodes->push_back(_node);
}

void node_graph::select_all(bool select)
{
    for (node *_node : *nodes)
        _node->select(select);
}
