#include <gl_view.hpp>

gl_view::gl_view()
{
    zoom_lock = false;
    zoom_scale = {1.0, 1.0};
}

gl_view::~gl_view()
{
}

void gl_view::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, .1, 0, 1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_MODELVIEW);
    glShadeModel(GL_SMOOTH);

    format().setSamples(10);
}

void gl_view::paintGL()
{
    // zoom y paneo
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = float(height()) / width();

    float left = coord.x() - zoom_scale.x() * 2;
    float right = coord.x() + zoom_scale.x() * 2;
    float top = (coord.y() * aspect) + (zoom_scale.y() * 2) * aspect;
    float bottom = (coord.y() * aspect) - (zoom_scale.y() * 2) * aspect;

    glOrtho(left, right, bottom, top, -1.f, 1.f);

    glMatrixMode(GL_MODELVIEW);
    //
    //
}

void gl_view::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}

QPointF gl_view::map_position(QPoint mouse_position)
{
    float aspect = float(height()) / width();

    QPointF coordinate = -get_coordinate(mouse_position) + coord;
    return {coordinate.x(), coordinate.y() * aspect};
}

QPointF gl_view::get_coordinate(QPoint cursor_position)
{
    // obtiene las cordenadas x, y del viewer del openGL, a partir de la posicion del cursor.
    float x = 2.0f * (cursor_position.x() + 0.5) / this->width() - 1.0;
    float y = 2.0f * (cursor_position.y() + 0.5) / this->height() - 1.0;

    float zoom_value_x = zoom_scale.x() * 2;
    float zoom_value_y = zoom_scale.y() * 2;

    x *= zoom_value_x;
    y *= zoom_value_y;

    return {-x, y};
}

void gl_view::set_zoom_lock(bool enable)
{
    zoom_lock = enable;
}

void gl_view::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0)
        zoom_scale = zoom_scale / 1.1;
    else
        zoom_scale = zoom_scale * 1.1;

    update();
}

void gl_view::mousePressEvent(QMouseEvent *event)
{
    click_position = event->pos();

    if (event->button() == Qt::MidButton)
    {
        if (qt::alt())
        {
            zooming = true;
            click_zoom_scale = zoom_scale;

            // se resta la posicion del click, a la cordenada,
            // y despues se suma en el 'mouseMoveEvent', y asi se logra el pundo de
            // anclaje donde esta el cursor.
            click_coord = coord - get_coordinate(click_position);
            //
        }
        else
        {
            click_coord = coord;
            panning = true;
        }
    }
    else if (event->button() == Qt::LeftButton)
    {
        if (qt::alt())
        {
            click_coord = coord;
            panning = true;
        }
    }
}

void gl_view::mouseReleaseEvent(QMouseEvent *event)
{
    panning = false;
    zooming = false;

    update();
}

void gl_view::mouseMoveEvent(QMouseEvent *event)
{
    if (panning)
    {
        // le resta la cordenada del click para que el paneo comience en 0
        QPointF coord_to_add = get_coordinate(event->pos()) - get_coordinate(click_position);
        //
        coord = click_coord + coord_to_add;
        update();
    }
    if (zooming)
    {
        float zoom_speed = 1.007;

        float zoom_x_to_add = click_position.x() - event->pos().x();
        float zoom_y_to_add = event->pos().y() - click_position.y();

        double scale_factor_x = pow(zoom_speed, zoom_x_to_add);
        double scale_factor_y = pow(zoom_speed, zoom_y_to_add);

        float zoom_scale_x = click_zoom_scale.x() * scale_factor_x;
        float zoom_scale_y = click_zoom_scale.y() * scale_factor_y;

        if (zoom_lock)
            zoom_scale_y = zoom_scale_x;

        zoom_scale = {zoom_scale_x, zoom_scale_y};

        // limitar zoom
        // if (zoom_scale < 0.2)
        //     zoom_scale = 0.2;
        // else if (zoom_scale > 7.0)
        //     zoom_scale = 7.0;
        //

        // punto de anclaje
        coord = click_coord + get_coordinate(click_position);
        //

        update();
    }
}