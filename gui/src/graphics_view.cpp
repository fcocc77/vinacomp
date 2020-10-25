#include <graphics_view.hpp>

graphics_view::graphics_view(/* args */)
{

    panning = false;
    pressed = false;
    zooming = false;
}

graphics_view::~graphics_view()
{
}

void graphics_view::mousePressEvent(QMouseEvent *event)
{
    pressed = true;
    if (event->button() == Qt::MidButton)
    {
        panning = true;
        panning_start_x = event->x();
        panning_start_y = event->y();
        event->accept();
        return;
    };
    event->ignore();
    QGraphicsView::mousePressEvent(event);
}

void graphics_view::mouseReleaseEvent(QMouseEvent *event)
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

void graphics_view::mouseMoveEvent(QMouseEvent *event)
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

void graphics_view::wheelEvent(QWheelEvent *event)
{
    // Zoom Factorl
    float zoom_in_factor = 1.25;
    float zoom_out_factor = 1 / zoom_in_factor;

    // Set Anchors
    this->setTransformationAnchor(QGraphicsView::NoAnchor);
    this->setResizeAnchor(QGraphicsView::NoAnchor);

    // Save the scene pos
    QPointF old_pos = this->mapToScene(event->pos());

    // Zoom
    float zoom_factor;
    if (event->delta() > 0)
        zoom_factor = zoom_in_factor;
    else
        zoom_factor = zoom_out_factor;
    this->scale(zoom_factor, zoom_factor);

    // Get the new position
    QPointF new_pos = this->mapToScene(event->pos());

    // Move scene to old position
    QPointF delta = new_pos - old_pos;
    this->translate(delta.x(), delta.y());
}

void graphics_view::keyPressEvent(QKeyEvent *event)
{

    int key = event->key();

    if (key == Qt::Key_Alt)
        zooming = true;

    if (key == 43)
    { //zoom in
        print("zoom in");
    }
    if (key == 45)
    { //zoom out
        print("zoom out");
    }
}
