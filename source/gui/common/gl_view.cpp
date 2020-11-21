#include <gl_view.hpp>

gl_view::gl_view(bool _lock_scale)
    : lock_scale(_lock_scale)
{
    set_default();
}

gl_view::~gl_view()
{
}

void gl_view::set_default()
{
    translate = {1.7, 1.7};
    scale = {1.0, 1.0};

    update();
}

void gl_view::set_transform(QPointF _translate, QPointF _scale)
{
    translate = _translate;
    scale = _scale;

    update();
}

void gl_view::set_ortho(float left, float right, float bottom, float top)
{
    float aspect = get_aspect();

    float translate_x = (right + left) / 2;
    float translate_y = ((top + bottom) / aspect) / 2;

    float scale_x = (right - left) / 4;
    float scale_y = ((top - bottom) / aspect) / 4;

    set_transform({translate_x, translate_y}, {scale_x, scale_y});
}

float gl_view::get_aspect()
{
    return float(height()) / width();
}

QPointF gl_view::get_coords(QPoint mouse_position)
{
    QPointF coordinate = -get_coordinate(mouse_position) + translate;
    return {coordinate.x(), coordinate.y() * get_aspect()};
}

QPointF gl_view::get_coordsf(QPointF mouse_position)
{
    return get_coords(mouse_position.toPoint());
}

QPointF gl_view::get_position(QPointF coordinate)
{
    // esta funcion es el reverso de 'get_coords'
    // obtiene la posicion en el visor, a partir de la cordenada de la escena.
    float x = coordinate.x();
    float y = coordinate.y() / get_aspect();

    x += -translate.x();
    y += -translate.y();

    float scale_value_x = scale.x() * 2;
    float scale_value_y = -scale.y() * 2;

    x /= scale_value_x;
    y /= scale_value_y;

    x = (x + 1.0) * this->width() / 2.0 - 0.5;
    y = (y + 1.0) * this->height() / 2.0 - 0.5;

    return {x, y};
}

QPointF gl_view::get_coordinate(QPoint cursor_position)
{
    // obtiene las cordenadas x, y del viewer del openGL, a partir de la posicion del cursor.
    float x = 2.0f * (cursor_position.x() + 0.5) / this->width() - 1.0;
    float y = 2.0f * (cursor_position.y() + 0.5) / this->height() - 1.0;

    float scale_value_x = scale.x() * 2;
    float scale_value_y = scale.y() * 2;

    x *= scale_value_x;
    y *= scale_value_y;

    return {-x, y};
}

bool gl_view::is_cursor_above(QPoint cursor_position, QPointF point, QPointF point2)
{
    int tolerance = 10;

    point = get_position(point);

    // bounding box
    int left = point.x() - tolerance;
    int right = point.x() + tolerance;
    int bottom = point.y() - tolerance;
    int top = point.y() + tolerance;
    //
    //

    // si es que hay un segundo puntos, puede servir para una linea o un rectangulo
    if (!point2.isNull())
    {
        point2 = get_position(point2);

        if (point2.x() < point.x())
            left = point2.x() - tolerance;

        if (point2.x() > point.x())
            right = point2.x() + tolerance;

        if (point2.y() < point.y())
            bottom = point2.y() - tolerance;

        if (point2.y() > point.y())
            top = point2.y() + tolerance;
    }
    //
    //

    int x = cursor_position.x();
    int y = cursor_position.y();

    if (x > left && x < right && y > bottom && y < top)
        return true;

    return false;
}

QPointF gl_view::get_scale()
{
    return scale;
}

QPointF gl_view::rotate_point(QPointF point, QPointF anchor_point, float angle, bool keep_aspect)
{
    // rota un punto alrededor de otro punto (punto de anclaje).
    float distance = qt::distance_points(point, anchor_point);

    angle = (M_PI * 2.0) * angle / 360.0;

    float x = distance * cosf(angle);
    float y = distance * sinf(angle);

    if (keep_aspect)
        y = y * (get_scale().y() / get_scale().x());

    x += anchor_point.x();
    y += anchor_point.y();

    return {x, y};
}

float gl_view::get_angle_two_points(QPointF point_a, QPointF point_b)
{
    // calcular la rotacion a partir de 2 puntos
    float delta_y = (point_a.y() - point_b.y());
    float delta_x = (point_b.x() - point_a.x());

    return atan2(delta_x, delta_y) * 180.0 / M_PI;
}

QList<float> gl_view::generate_coord_range(
    float separation,
    Qt::Orientation orientation,
    QColor &color,
    QPointF life_range,
    bool separation_by_coord)
{
    // retorna un vacio si es que la separacion de 2 cordenadas no esta dentro del rango de vida,

    float scale_x = get_scale().x();
    float scale_y = get_scale().y();

    float life;
    if (orientation == Qt::Vertical)
    {
        if (separation_by_coord)
            life = scale_y;
        else
            // calcula la vida en relacion a la separacion en el visor, tomando como promedio 1000px
            life = scale_y * 1000 / height();
    }
    else
    {
        if (separation_by_coord)
            life = scale_x;
        else
            life = scale_x * 1000 / width();
    }

    life /= separation;

    float life_start = life_range.x();
    float life_end = life_range.y();

    if (life > life_end || life < life_start)
        return {};
    //
    //

    // cambia el nivel de opacidad del color entrante, con un  'fade out' a negro
    float alpha = 1.0 - (life - life_start) / abs(life_start - life_end);
    color.setRed(color.red() * alpha);
    color.setGreen(color.green() * alpha);
    color.setBlue(color.blue() * alpha);
    //
    //

    // genera un rango de cordenadas, pero estos valores son los que se ven dentro de cuadro,
    // los valores que esta fuera de cuadro no los muestra, esto sirve para que
    // las iteraciones no sean tan largas ya que no renderiza las que estan fuera de cuadro.
    int out_frame = 50;
    QPointF top_left_point = get_coords({-out_frame, -out_frame});
    QPointF down_right_point = get_coords({width() + out_frame, height() + out_frame});

    float a_limit, b_limit;
    if (orientation == Qt::Vertical)
    {
        a_limit = down_right_point.y();
        b_limit = top_left_point.y();
    }
    else
    {
        a_limit = top_left_point.x();
        b_limit = down_right_point.x();
    }

    float range = (abs(a_limit - b_limit) / separation) * separation;

    float start = a_limit / separation;
    float end = (range + a_limit) / separation;

    QList<float> coords;
    for (int i = start; i < end; i++)
        coords.push_back(i * separation);

    return coords;
}

void gl_view::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0)
        scale = scale / 1.1;
    else
        scale = scale * 1.1;

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
            click_scale = scale;

            // se resta la posicion del click, a la cordenada,
            // y despues se suma en el 'mouseMoveEvent', y asi se logra el pundo de
            // anclaje donde esta el cursor.
            click_translate = translate - get_coordinate(click_position);
            //
        }
        else
        {
            click_translate = translate;
            panning = true;
        }
    }
    else if (event->button() == Qt::LeftButton)
    {
        if (qt::alt())
        {
            click_translate = translate;
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
        translate = click_translate + coord_to_add;
        update();
    }
    if (zooming)
    {
        float scale_speed = 1.007;

        float scale_x_to_add = click_position.x() - event->pos().x();
        float scale_y_to_add = event->pos().y() - click_position.y();

        double scale_factor_x = pow(scale_speed, scale_x_to_add);
        double scale_factor_y = pow(scale_speed, scale_y_to_add);

        float scale_x = click_scale.x() * scale_factor_x;
        float scale_y = click_scale.y() * scale_factor_y;

        if (lock_scale)
            scale_y = scale_x;

        float min = 0.02;
        float max = 100000;

        // limitar zoom Horizontal
        if (scale_x < min)
            scale_x = min;
        else if (scale_x > max)
            scale_x = max;
        //

        // limitar zoom Vertical
        if (scale_y < min)
            scale_y = min;
        else if (scale_y > max)
            scale_y = max;
        //

        scale = {scale_x, scale_y};

        // punto de anclaje
        translate = click_translate + get_coordinate(click_position);
        //

        update();
    }
}