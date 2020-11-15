#include <graphics_view.hpp>

graphics_view::graphics_view(/* args */)
{
    panning = false;
    zooming = false;

    this->setTransformationAnchor(QGraphicsView::NoAnchor);
}

graphics_view::~graphics_view()
{
}

void graphics_view::mousePressEvent(QMouseEvent *event)
{
    click_position = event->pos();

    if (event->button() == Qt::MidButton)
    {
        if (qt::alt())
        {
            zooming = true;
            last_mouse_position = event->pos();
        }
        else
        {
            panning = true;
            panning_start_x = event->x();
            panning_start_y = event->y();
        }
        event->accept();
        return;
    }
    else if (event->button() == Qt::LeftButton)
    {
        if (qt::alt())
        {
            panning = true;
            panning_start_x = event->x();
            panning_start_y = event->y();
            event->accept();
            return;
        }
    }

    QGraphicsView::mousePressEvent(event);
}

void graphics_view::mouseReleaseEvent(QMouseEvent *event)
{
    panning = false;
    zooming = false;

    QGraphicsView::mouseReleaseEvent(event);
}

void graphics_view::mouseMoveEvent(QMouseEvent *event)
{
    if (zooming)
    {
        int delta = 2 * ((event->x() - last_mouse_position.x()) - (event->y() - last_mouse_position.y()));
        zoom(delta, this->click_position);

        last_mouse_position = event->pos();
        event->accept();
    }

    else if (panning)
    {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - panning_start_x));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - panning_start_y));
        panning_start_x = event->x();
        panning_start_y = event->y();
        event->accept();
        return;
    }

    QGraphicsView::mouseMoveEvent(event);
}

void graphics_view::zoom(int delta, QPoint position)
{
    QPointF old_pos = mapToScene(position);
    float scale_speed = 1.002;

    double scale_factor = pow(scale_speed, delta);
    QTransform _transform = transform();
    double current_scale_factor = _transform.mapRect(QRectF(0, 0, 1, 1)).width();
    double new_scale_factor = current_scale_factor * scale_factor;

    // limita el zoom in y out
    if (((new_scale_factor < 0.1) && (scale_factor < 1.)) || ((new_scale_factor > 10) && (scale_factor > 1.)))
        return;
    //
    //

    this->scale(scale_factor, scale_factor);

    QPointF new_pos = this->mapToScene(position);
    QPointF _delta = new_pos - old_pos;
    this->translate(_delta.x(), _delta.y());
}

void graphics_view::wheelEvent(QWheelEvent *event)
{
    zoom(event->delta(), event->pos());
}

void graphics_view::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();

    if (key == Qt::Key_F)
        this->setTransform(QTransform()); //fit
}

void graphics_view::keyReleaseEvent(QKeyEvent *event)
{
    panning = false;
    zooming = false;
}

void graphics_view::focusOutEvent(QFocusEvent *event)
{
    panning = false;
    zooming = false;
}