#include <curve_view.hpp>

void curve_view::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
}

void curve_view::paintGL()
{
    gl_view::paintGL();

    draw_grid();
    draw_coordinate_numbers();
    draw_curve();
    draw_selector();
    draw_resize_box();
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

void curve_view::draw_bezier(key_frame src_key, key_frame dst_key)
{
    QLineF src_handler = get_handler_points(src_key, {}, dst_key, true);
    QLineF dst_handler = get_handler_points(dst_key, src_key, {}, true);

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

void curve_view::draw_curve()
{
    for (auto curve : curves)
    {
        // Infinite Lines
        draw_line({-1000000, curve.first().pos.y()}, curve.first().pos, Qt::red);
        draw_line(curve.last().pos, {1000000, curve.last().pos.y()}, Qt::red);
        //
        //

        for (int i = 0; i < curve.count(); i++)
        {
            key_frame key = curve.value(i);
            key_frame previous_key = curve.value(i - 1);
            key_frame next_key = curve.value(i + 1);

            // Beziers
            if (previous_key.init)
                draw_bezier(previous_key, key);
            //
            //

            // Point
            if (key.selected)
                draw_point(key.pos);
            else
                draw_point(key.pos, Qt::red);
            //
            //

            // Handler
            if (key.selected)
            {
                QLineF handler = get_handler_points(key, previous_key, next_key);

                draw_line(handler.p1(), handler.p2(), Qt::red);
                draw_point(handler.p1());
                draw_point(handler.p2());
            }
            //
            //
        }
    }
}

void curve_view::draw_selector()
{
    if (!selecting)
        return;

    draw_box(selector, {20, 20, 20}, {100, 100, 100});
}

void curve_view::draw_resize_box()
{
    QLineF line = {{0.5, 0.5}, {1, 1}};
    draw_box(line, {20, 30, 20}, {100, 200, 100});
}