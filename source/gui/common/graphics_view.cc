#include <graphics_view.h>
#include <qt.h>
#include <util.h>

graphics_view::graphics_view()
    : panning(false)
    , zooming(false)
{
    this->setTransformationAnchor(QGraphicsView::NoAnchor);
}

graphics_view::~graphics_view() {}

QRectF graphics_view::get_view_rect() const
{
    // obtiene el rectangulo del viewport actual, ajustando un poco para que sea
    // mas exacto, como el aspecto entre el alto y ancho
    QRectF rect = mapToScene(viewport()->geometry()).boundingRect();
    float aspect = (float)height() / float(width());

    int x = rect.x() + 1;
    int y = rect.y() + 1;
    int width = rect.width();
    int height = width * aspect;

    return QRectF(x, y, width, height);
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
    freeze_current_rect();

    QGraphicsView::mouseReleaseEvent(event);
}

void graphics_view::mouseMoveEvent(QMouseEvent *event)
{
    if (zooming)
    {
        int delta =
            2 * ((event->x() - last_mouse_position.x()) - (event->y() - last_mouse_position.y()));
        zoom(delta, this->click_position);

        last_mouse_position = event->pos();
        event->accept();
    }

    else if (panning)
    {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() -
                                        (event->x() - panning_start_x));
        verticalScrollBar()->setValue(verticalScrollBar()->value() -
                                      (event->y() - panning_start_y));
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
    float scale_speed = 1.004;

    double scale_factor = pow(scale_speed, delta);
    QTransform _transform = transform();
    double current_scale_factor = _transform.mapRect(QRectF(0, 0, 1, 1)).width();
    double new_scale_factor = current_scale_factor * scale_factor;

    // limita el zoom in y out
    if (((new_scale_factor < 0.1) && (scale_factor < 1.)) ||
        ((new_scale_factor > 10) && (scale_factor > 1.)))
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
    freeze_current_rect();
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

void graphics_view::restore_rect(QRectF rect)
{
    // ajusta el viewport al rect, respetando el actual aspecto
    float aspect = (float)height() / float(width());

    float width = rect.width();
    float height = width * aspect;

    float height_diff = (rect.height() - height) / 2;

    float x = rect.x();
    float y = rect.y() + height_diff;

    rect = QRectF(x, y, width, height);

    fitInView(rect);
}
