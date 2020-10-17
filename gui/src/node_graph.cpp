#include <node_graph.hpp>

node_graph::node_graph(/* args */)
{

    QGraphicsScene *scene = new QGraphicsScene();
    QGraphicsView *graphics_view = new QGraphicsView();

    graphics_view->setScene(scene);

    QHBoxLayout *layout = new QHBoxLayout();

    node *node_a = new node();
    node *node_b = new node();

    // scene->addWidget(node_b);

    QBrush red(Qt::red);
    QPen pen(Qt::black);
    pen.setWidth(1);

    QGraphicsRectItem *rectangle = new QGraphicsRectItem();
    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget();
    QLabel *label = new QLabel("Nodo");

    // proxy->setWidget(label);

    // scene->addRect(-100, 100, 50, 50, pen, red);
    rectangle = scene->addRect(100, 100, 50, 50, pen, Qt::red);
    rectangle->setFlag(QGraphicsItem::ItemIsMovable);
    rectangle->setRect(0, 0, 77, 26);

    // proxy->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

    // proxy->setPos(100, 500);
    // scene->addItem(proxy);

    layout->addWidget(graphics_view);
    this->setLayout(layout);
}

node_graph::~node_graph()
{
}