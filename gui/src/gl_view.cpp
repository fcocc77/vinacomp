#include <gl_view.hpp>

gl_view::gl_view()
{
    zoom_lock = false;
    set_default();
}

gl_view::~gl_view()
{
}

void gl_view::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
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

void gl_view::set_default()
{
    coord = {2, 2};
    zoom_scale = {1.3, 1.3};

    update();
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

QPointF gl_view::get_position(QPointF coordinate)
{
    // esta funcion es el reverso de 'map_position'
    // obtiene la posicion en el visor, a partir de la cordenada de la escena.
    float aspect = float(height()) / width();

    float x = coordinate.x();
    float y = coordinate.y() / aspect;

    x += -coord.x();
    y += -coord.y();

    float zoom_value_x = zoom_scale.x() * 2;
    float zoom_value_y = -zoom_scale.y() * 2;

    x /= zoom_value_x;
    y /= zoom_value_y;

    x = (x + 1.0) * this->width() / 2.0 - 0.5;
    y = (y + 1.0) * this->height() / 2.0 - 0.5;

    return {x, y};
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

QPointF gl_view::get_scale()
{
    return zoom_scale;
}

void gl_view::draw_line(QPointF src, QPointF dst, QColor color)
{
    glBegin(GL_LINES);
    glColor4f(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0, 0.0);
    glVertex2f(src.x(), src.y());
    glVertex2f(dst.x(), dst.y());
    glEnd();
}

void gl_view::draw_text(QString text, QColor color, QPointF coords, QPointF viewer_position)
{
    int font_size = 10;

    QPainter painter(this);
    painter.setPen(color);
    painter.setFont(QFont("Arial", font_size));

    // posicion en el visor
    QPointF position_from_coords = get_position(coords);
    //

    // Determina la posicion si es desde el visor o desde las cordenadas
    float x, y;
    if (viewer_position.x() >= 0)
        x = viewer_position.x();
    else
        x = position_from_coords.x();

    if (viewer_position.y() >= 0)
        y = viewer_position.y();
    else
        y = position_from_coords.y();
    //
    //

    painter.drawText(x, y, 50, font_size, Qt::AlignCenter, text);
    painter.end();
}

QList<float> gl_view::generate_coord_range(
    float separation,
    Qt::Orientation orientation,
    QColor &color,
    QPointF life_range)
{
    // retorna un vacio si es que la separacion de 2 cordenadas no esta dentro del rango de vida,
    float zoom;
    if (orientation == Qt::Vertical)
        zoom = get_scale().y();
    else
        zoom = get_scale().x();

    float scale = zoom / separation;

    float life_start = life_range.x();
    float life_end = life_range.y();

    if (scale > life_end || scale < life_start)
        return {};
    //
    //

    // cambia el nivel de opacidad del color entrante
    float level = 1.0 - (1.0 * (scale - life_start) / abs(life_start - life_end));

    int red = color.red() * level;
    int green = color.green() * level;
    int blue = color.blue() * level;

    color = QColor(red, green, blue);
    //
    //

    // genera un rango de cordenadas, pero estos valores son los que se ven dentro de cuadro,
    // los valores que esta fuera de cuadro no los muestra, esto sirve para que
    // las iteraciones no sean tan largas ya que no renderiza las que estan fuera de cuadro.

    int out_frame = 50;
    QPointF top_left_point = map_position({-out_frame, -out_frame});
    QPointF down_right_point = map_position({width() + out_frame, height() + out_frame});

    float up_limit = top_left_point.y();
    float left_limit = top_left_point.x();

    float down_limit = down_right_point.y();
    float right_limit = down_right_point.x();

    float a_limit, b_limit;
    if (orientation == Qt::Vertical)
    {
        a_limit = down_limit;
        b_limit = up_limit;
    }
    else
    {
        a_limit = left_limit;
        b_limit = right_limit;
    }

    float range = (abs(a_limit - b_limit) / separation) * separation;
    float start = a_limit;
    float end = range + start;

    start /= separation;
    end /= separation;

    QList<float> coords;
    for (int i = start; i < end; i++)
        coords.push_back(i * separation);

    return coords;
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

        float min = 0.02;
        float max = 100000;

        // limitar zoom Horizontal
        if (zoom_scale_x < min)
            zoom_scale_x = min;
        else if (zoom_scale_x > max)
            zoom_scale_x = max;
        //

        // limitar zoom Vertical
        if (zoom_scale_y < min)
            zoom_scale_y = min;
        else if (zoom_scale_y > max)
            zoom_scale_y = max;
        //

        zoom_scale = {zoom_scale_x, zoom_scale_y};

        // punto de anclaje
        coord = click_coord + get_coordinate(click_position);
        //

        update();
    }
}