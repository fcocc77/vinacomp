#include <gl_view.h>

gl_view::gl_view(bool _lock_scale)
    : lock_scale(_lock_scale)
	, panning(false)
    , zooming(false)
	, pressed(false)
	, center_button(false)

{
    this->setMouseTracking(true);
    this->setTabletTracking(true);

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

QPointF gl_view::get_coords(QPoint mouse_position) const
{
    QPointF coordinate = -get_coordinate(mouse_position) + translate;
    return {coordinate.x(), coordinate.y() * get_aspect()};
}

QPointF gl_view::get_position(QPointF coordinate) const
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

QPointF gl_view::get_coordinate(QPoint cursor_position) const
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

bool gl_view::cursor_above_rect(QPoint cursor_position, QPointF point, QPointF point2) const
{
	// ! el rect lo genera a partir de dos puntos en diagonal
    int tolerance = 10;

    point = get_position(point);

    // bounding box
    int left = point.x() - tolerance;
    int right = point.x() + tolerance;
    int bottom = point.y() - tolerance;
    int top = point.y() + tolerance;
    //
    //

	point2 = get_position(point2);

	if (point2.x() < point.x())
		left = point2.x() - tolerance;

	if (point2.x() > point.x())
		right = point2.x() + tolerance;

	if (point2.y() < point.y())
		bottom = point2.y() - tolerance;

	if (point2.y() > point.y())
		top = point2.y() + tolerance;

    int x = cursor_position.x();
    int y = cursor_position.y();

    if (x > left && x < right && y > bottom && y < top)
        return true;

    return false;
}

bool gl_view::cursor_above_point(QPoint cursor, QPointF point, int tolerance) const
{
    point = get_position(point);

    // bounding box
    int left = point.x() - tolerance;
    int right = point.x() + tolerance;
    int bottom = point.y() - tolerance;
    int top = point.y() + tolerance;
    //
    //

    int x = cursor.x();
    int y = cursor.y();

    if (x > left && x < right && y > bottom && y < top)
        return true;

    return false;
}

bool gl_view::cursor_above_line(QPoint cursor, QLineF line) const
{
    int tolerance = 2;

	QPointF p1 = get_position(line.p1());
	QPointF p2 = get_position(line.p2());

	float distance_p1 = qt::distance_points(cursor, p1);
	float distance_p2 = qt::distance_points(cursor, p2);

	float distance = distance_p1 + distance_p2;
	float line_length = qt::distance_points(p1, p2);

	float diff = abs(distance - line_length);

	return diff < tolerance;
}

QPointF gl_view::rotate_point(QPointF point, QPointF anchor_point, float angle, bool keep_aspect, bool keep_offset) const
{
    // rota un punto alrededor de otro punto (punto de anclaje).
    float distance = qt::distance_points(point, anchor_point);

	if (keep_offset)
		angle += get_angle_two_points(point, anchor_point) + 90;

    angle = (M_PI * 2.0) * angle / 360.0;

    float x = distance * cosf(angle);
    float y = distance * sinf(angle);

    if (keep_aspect)
        y = y * (get_scale().y() / get_scale().x());

    x += anchor_point.x();
    y += anchor_point.y();

    return {x, y};
}

float gl_view::get_angle_two_points(QPointF point_a, QPointF point_b) const
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
    bool separation_by_coord,
	int _life)
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
            life = scale_y * _life / height();
    }
    else
    {
        if (separation_by_coord)
            life = scale_x;
        else
            life = scale_x * _life / width();
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
    int out_frame = 10;
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
