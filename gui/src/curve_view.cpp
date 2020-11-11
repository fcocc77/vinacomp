#include <curve_view.hpp>

curve_view::curve_view(/* args */)
{
    this->setMouseTracking(true);
    create_curve();
}

curve_view::~curve_view()
{
}

void curve_view::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
}

void curve_view::draw_circle()
{
    int num_segments = 100;
    float radio = 1.0;

    glBegin(GL_LINE_STRIP);
    glColor4f(1, 0, 0, 0);

    float segment = 360.0 / num_segments;
    float angle = 0;
    for (int i = 0; i <= num_segments; i++)
    {
        QPointF point = rotate_point({radio, 0}, {0.0, 0.0}, angle);
        glVertex2f(point.x(), point.y());

        angle += segment;
    }
    glEnd();
}

void curve_view::draw_grid()
{
    int out_frame = 50;
    QPointF top_left_point = get_coords({-out_frame, -out_frame});
    QPointF down_right_point = get_coords({width() + out_frame, height() + out_frame});

    float up_limit = top_left_point.y();
    float left_limit = top_left_point.x();

    float down_limit = down_right_point.y();
    float right_limit = down_right_point.x();

    auto horizontal_lines = [=](float separation, QColor color) {
        for (float value : generate_coord_range(separation, Qt::Vertical, color, {0, 40}, true))
            draw_line({left_limit, value}, {right_limit, value}, color);
    };

    auto vertical_lines = [=](float separation, QColor color) {
        for (float value : generate_coord_range(separation, Qt::Horizontal, color, {0, 40}, true))
            draw_line({value, down_limit}, {value, up_limit}, color);
    };

    // Eje X e Y
    draw_line({left_limit, 0.0}, {right_limit, 0.0}, QColor(120, 30, 30));
    draw_line({0.0, down_limit}, {0.0, up_limit}, QColor(20, 120, 20));
    //
    //

    // Activa la la mezcla en openGL, para que siempre se vea la linea mas luminosa 'GL_MAX'.
    glEnable(GL_BLEND);
    glBlendEquation(GL_MAX);
    //
    //

    QList<float> separations = {0.005, 0.05, 0.5, 5, 50, 500, 5000, 50000};
    for (float separation : separations)
    {
        horizontal_lines(separation, QColor(0, 60, 10));
        vertical_lines(separation, QColor(0, 60, 10));
    }
}

void curve_view::draw_coordinate_numbers()
{
    auto vertical_numbers = [=](float separation) {
        QColor color = QColor(0, 200, 0);
        for (float value : generate_coord_range(separation, Qt::Vertical, color, {0.0, 15}))
            draw_text(QString::number(value), color, {0, value}, {20, -1});
    };

    auto horizontal_numbers = [=](float separation) {
        QColor color = QColor(200, 0, 0);
        for (float value : generate_coord_range(separation, Qt::Horizontal, color, {0.0, 10}))
            draw_text(QString::number(value), color, {value, 0}, {-1, height() - 20});
    };

    QList<float> value_separations = {0.01, 0.05, 0.1, 0.5, 1, 5, 10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000};
    for (float separation : value_separations)
        vertical_numbers(separation);

    QList<float> time_separations = {1, 5, 10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000};
    for (float separation : time_separations)
        horizontal_numbers(separation);
}

QLineF curve_view::get_handler_points(key_frame key, bool infinite)
{
    // obtiene los dos puntos del manejador
    float separation = 0.2;

    float point_a_x = key.pos.x() - separation;
    float point_b_x = key.pos.x() + separation;

    QPointF point_a = {point_a_x, key.pos.y()};
    QPointF point_b = {point_b_x, key.pos.y()};

    // genera el punto vertical infinito donde apunta el manejador
    float tangent = tan(key.angle * M_PI / 180);
    QPointF infinite_point_a = {point_a.x(), point_a.y() + (tangent * separation)};
    QPointF infinite_point_b = {point_b.x(), point_b.y() - (tangent * separation)};
    //

    if (!infinite)
    {
        // antes de rotar el manejador, transforma el punto de key y los
        // 2 puntos del manejador de cordenadas a puntos en la
        // position del visor, con esto logramos que el manejador
        // siempre quede del mismo tamaño.
        QPointF view_point_a = get_position(point_a);
        QPointF view_point_b = get_position(point_b);

        QPointF view_infinite = get_position(infinite_point_a);
        QPointF view_anchor_point = get_position(key.pos);

        float angle = get_angle_two_points(view_infinite, view_anchor_point) - 90;

        view_point_a = rotate_point(view_point_a, view_anchor_point, angle - 180);
        view_point_b = rotate_point(view_point_b, view_anchor_point, angle);

        point_a = get_coords({view_point_a.x(), view_point_a.y()});
        point_b = get_coords({view_point_b.x(), view_point_b.y()});
    }

    if (infinite)
        return {infinite_point_a, infinite_point_b};
    else
        return {point_a, point_b};
}

void curve_view::draw_curve()
{
    for (auto curve : curves)
    {
        // Infinite Lines
        draw_line({-1000000, curve.first().pos.y()}, curve.first().pos, Qt::red);
        draw_line(curve.last().pos, {1000000, curve.last().pos.y()}, Qt::red);
        //
        //

        // Beziers
        key_frame last_key;
        bool is_first = true;
        for (key_frame key : curve)
        {
            if (!is_first)
                draw_bezier(last_key, key);

            last_key = key;
            is_first = false;
        }
        //
        //

        // Handler
        for (key_frame key : curve)
        {
            QLineF handler = get_handler_points(key);

            draw_line(handler.p1(), handler.p2(), Qt::red);
            draw_point(handler.p1());
            draw_point(handler.p2());
        }
        //

        // Points
        for (key_frame key : curve)
            draw_point(key.pos);
        //
    }
}

QPointF curve_view::cubic_bezier(
    QPointF p1, QPointF p2,
    QPointF p3, QPointF p4,
    float value)
{

    // Algoritmo bezier
    QPointF L1 = ((1 - value) * p1) + (value * p2);
    QPointF L2 = ((1 - value) * p2) + (value * p3);
    QPointF L3 = ((1 - value) * p3) + (value * p4);

    QPointF Q1 = ((1 - value) * L1) + (L2 * value);
    QPointF Q2 = ((1 - value) * L2) + (L3 * value);

    return ((1 - value) * Q1) + (Q2 * value);
    //
}

void curve_view::draw_bezier(key_frame src_key, key_frame dst_key)
{
    QLineF src_handler = get_handler_points(src_key, true);
    QLineF dst_handler = get_handler_points(dst_key, true);

    draw_point(dst_handler.p1());
    glBegin(GL_LINE_STRIP);
    glColor3f(1, 1, 0);

    int segments = 100;
    float segment = 1.0 / segments;
    float t = 0;

    for (int i = 0; i <= segments; i++)
    {
        QPointF point = cubic_bezier(src_key.pos, src_handler.p2(), dst_handler.p1(), dst_key.pos, t);
        glVertex2f(point.x(), point.y());

        t += segment;
    }

    glEnd();
}

void curve_view::create_curve()
{
    key_frame key1 = {{0.1, 0.2}, 0, 0};
    key_frame key2 = {{0.5, 1}, 45, 0};
    key_frame key3 = {{1, 0.3}, 0, 0};

    curves.insert("translate_x", {key1, key2, key3});

    update();
}

void curve_view::paintGL()
{
    gl_view::paintGL();

    draw_grid();
    draw_coordinate_numbers();
    draw_curve();
}

void curve_view::key_press(QPoint cursor_position)
{
    // si el click del mouse fue presionado en algun keyframe o en
    // alguno de los 2 puntos del manejador, los asigna a las variables de 'drag'

    int draw_tolerance = 10; // Pixels
    for (auto curve : curves.keys())
    {
        auto keys = curves[curve];
        for (int i = 0; i < keys.count(); i++)
        {
            key_frame key = keys[i];
            QLineF handler = get_handler_points(key);

            drag_index = i;
            drag_curve = curve;
            drag_handler = 0;

            // verifica si el click se hizo en el key point
            QPointF key_position = get_position(key.pos);
            float distance = qt::distance_points(key_position, cursor_position);
            if (distance < draw_tolerance)
                is_drag = true;
            //
            //

            // verifica si el click se hizo en alguno de los 2 puntos manejadores
            distance = qt::distance_points(get_position(handler.p1()), cursor_position);
            if (distance < draw_tolerance)
            {
                drag_handler = 1;
                is_drag = true;
            }
            distance = qt::distance_points(get_position(handler.p2()), cursor_position);
            if (distance < draw_tolerance)
            {
                drag_handler = 2;
                is_drag = true;
            }
            //
            //

            if (is_drag)
                break;
        }
    }
}

void curve_view::mousePressEvent(QMouseEvent *event)
{
    key_press(event->pos());
    gl_view::mousePressEvent(event);
}

void curve_view::mouseReleaseEvent(QMouseEvent *event)
{
    is_drag = false;
    gl_view::mouseReleaseEvent(event);
}

void curve_view::mouseMoveEvent(QMouseEvent *event)
{
    if (is_drag)
    {
        if (!qt::alt())
        {
            if (curves.contains(drag_curve))
            {
                key_frame &key = curves[drag_curve][drag_index];

                if (drag_handler == 1)
                    key.angle = 90 - get_angle_two_points(get_coords(event->pos()), key.pos);

                else if (drag_handler == 2)
                    key.angle = 270 - get_angle_two_points(get_coords(event->pos()), key.pos);
                else
                    key.pos = get_coords(event->pos());

                update();
            }
        }
    }

    gl_view::mouseMoveEvent(event);
}