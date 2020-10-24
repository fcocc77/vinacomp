#include <node_graph.hpp>

node_graph::node_graph(/* args */)
{
    this->setObjectName("node_graph");

    scene = new QGraphicsScene();

    this->setScene(scene);

    node *node_a = new node();
    node *node_b = new node();

    scene->addWidget(node_b);

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

    // desabilita el scroll
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //
    //

    scene->setSceneRect(-500000, -500000, 1000000, 1000000);

    panning = false;
}

node_graph::~node_graph()
{
}

void node_graph::mousePressEvent(QMouseEvent *event)
{

    if (event->button() == Qt::MidButton)
    {
        panning = true;
        panning_start_x = event->x();
        panning_start_y = event->y();
        event->accept();
        return;
    }
    event->ignore();
}
void node_graph::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MidButton)
    {
        panning = false;
        event->accept();
        return;
    }
    event->ignore();
}

void node_graph::mouseMoveEvent(QMouseEvent *event)
{
    if (panning)
    {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - panning_start_x));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - panning_start_y));
        panning_start_x = event->x();
        panning_start_y = event->y();
        event->accept();
        return;
    }
    event->ignore();
}

void node_graph::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0)
        scale(1.25, 1.25);
    else
        scale(0.8, 0.8);
}