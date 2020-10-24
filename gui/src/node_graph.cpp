#include <node_graph.hpp>

node_graph::node_graph(/* args */)
{
    this->setObjectName("node_graph");

    scene = new QGraphicsScene();

    this->setScene(scene);

    node *node_a = new node();
    node *node_b = new node();

    QGraphicsProxyWidget *proxy = scene->addWidget(node_a);

    QPen pen(Qt::black);
    pen.setWidth(1);

    rectangle = scene->addRect(100, 100, 50, 50, pen, Qt::red);
    rectangle->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    rectangle->setRect(0, 0, 77, 26);

    proxy->setParentItem(rectangle);

    // desabilita el scroll
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //
    //

    scene->setSceneRect(-500000, -500000, 1000000, 1000000);

    panning = false;

    pressed = false;
}

node_graph::~node_graph()
{
}

void node_graph::paintEvent(QPaintEvent *event)
{

    QPainter painter(this->viewport());
    QPen line_pen(Qt::black);
    line_pen.setWidth(3);
    painter.setPen(line_pen);

    int x = rectangle->x();
    int y = rectangle->y();

    int pos_x = -horizontalScrollBar()->value();
    int pox_y = -verticalScrollBar()->value();

    painter.drawLine(pos_x + x, pox_y + y, 200, 200);

    QGraphicsView::paintEvent(event);
}

void node_graph::mousePressEvent(QMouseEvent *event)
{
    pressed = true;
    if (event->button() == Qt::MidButton)
    {
        panning = true;
        panning_start_x = event->x();
        panning_start_y = event->y();
        event->accept();
        return;
    }
    event->ignore();
    QGraphicsView::mousePressEvent(event);
}

void node_graph::mouseReleaseEvent(QMouseEvent *event)
{
    pressed = false;
    if (event->button() == Qt::MidButton)
    {
        panning = false;
        event->accept();
        return;
    }
    QGraphicsView::mouseReleaseEvent(event);
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
    QGraphicsView::mouseMoveEvent(event);
    event->ignore();
}

void node_graph::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0)
        scale(1.25, 1.25);
    else
        scale(0.8, 0.8);
}