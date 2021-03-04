#include <curve_view.h>

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
    draw_transform_box();
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

    aa_enable(false);

    // Eje X e Y
    draw_line({left_limit, 0.0}, {right_limit, 0.0}, QColor(120, 30, 30));
    draw_line({0.0, down_limit}, {0.0, up_limit}, QColor(20, 120, 20));
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

void curve_view::draw_bezier(key_frame *src_key, key_frame *dst_key)
{
    QPointF src_handler = get_handler_points(src_key, true).p2();
    QPointF dst_handler = get_handler_points(dst_key, true).p1();

    if (src_key->right_interpolation() == 0)
        src_handler = src_key->pos();

    if (dst_key->left_interpolation() == 0)
        dst_handler = dst_key->pos();

    glBegin(GL_LINE_STRIP);
    QColor c = src_key->get_color();
    glColor3f(c.red() / 255, c.green() / 255, c.blue() / 255);

    int segments = 100;
    float segment = 1.0 / segments;
    float t = 0;

    for (int i = 0; i <= segments; i++)
    {
        QPointF point = cubic_bezier(src_key->pos(), src_handler, dst_handler, dst_key->pos(), t);
        glVertex2f(point.x(), point.y());

        t += segment;
    }

    glEnd();
}

void curve_view::draw_curve()
{
    for (auto keys : curves)
    {
        aa_enable(true);

        // Infinite Lines
        float infinite_x = 100000000;
        float infinite_first_y = tan(keys.first()->get_left_angle() * M_PI / 180) * infinite_x;
        float infinite_last_y = tan(keys.last()->get_right_angle() * M_PI / 180) * infinite_x;

        draw_line({-infinite_x, infinite_first_y}, keys.first()->pos(), keys.first()->get_color());
        draw_line(keys.last()->pos(), {infinite_x, infinite_last_y}, keys.last()->get_color());
        //
        //

        // crear bezier o linea
        for (key_frame *key : keys)
        {
            key_frame *previous_key = get_previous_key(key);

            // Beziers
            if (previous_key)
            {
                // si los 2 keyframes estan en 'linear', crea una linea recta, asi ahorramos recursos
                if (key->left_interpolation() == 0 and previous_key->right_interpolation() == 0)
                    draw_line(previous_key->pos(), key->pos(), key->get_color());
                else
                    draw_bezier(previous_key, key);
            }
            //
            //
        }
        //
        //

        // crear handlers
        for (key_frame *key : keys)
        {
            // Handler
            if (key->selected())
            {
                QLineF handler = get_handler_points(key);

                // si el keyframe es 'broken', le una linea puenteada al handler
                if (key->is_broken())
                    draw_dashed_line({handler.p1(), key->pos()}, Qt::red, 3);
                else
                    draw_line(handler.p1(), key->pos(), Qt::red);

                if (key->is_broken())
                    draw_dashed_line({handler.p2(), key->pos()}, Qt::red, 3);
                else
                    draw_line(handler.p2(), key->pos(), Qt::red);
                //

                draw_point(handler.p1(), Qt::red);
                draw_point(handler.p2(), Qt::red);

                if (text_visible)
                {
                    draw_text(qt::float_to_string(key->get_left_angle(), 1) + "°",
                              Qt::white, handler.p1(),
                              {-1, -1}, 9, Qt::AlignRight, {5, -10});

                    draw_text(qt::float_to_string(key->get_right_angle(), 1) + "°",
                              Qt::white, handler.p2(),
                              {-1, -1}, 9, Qt::AlignRight, {5, -10});
                }
            }
            //
            //

            // Point
            if (key->selected())
                draw_point(key->pos());
            else
                draw_point(key->pos(), key->get_color());
            //
            //

            // Text
            if (key->selected())
            {
                if (text_visible)
                    draw_text("x:" + qt::float_to_string(key->x(), 2) + "  y:" + qt::float_to_string(key->y(), 2),
                              Qt::white, key->pos(),
                              {-1, -1}, 9, Qt::AlignRight, {5, 10});
            }
        }
    }
}

void curve_view::draw_selector()
{
    if (!selecting)
        return;

    draw_box(selector, {20, 20, 20}, {100, 100, 100});
}

void curve_view::draw_transform_box()
{
    if (!transform_box_visible)
        return;

    QColor color = {100, 100, 100};

    QPointF bottom_left = transform_box.p1();
    QPointF top_right = transform_box.p2();
    QPointF bottom_right = {top_right.x(), bottom_left.y()};
    QPointF top_left = {bottom_left.x(), top_right.y()};

    draw_box({bottom_left, top_right}, {10, 10, 10}, color);
    //
    //

    // Vertices
    int size = 6;
    int smooth = false;
    draw_point(bottom_left, color, size, smooth);
    draw_point(bottom_right, color, size, smooth);
    draw_point(top_left, color, size, smooth);
    draw_point(top_right, color, size, smooth);
    //
    //

    // + Central
    float center_x = (bottom_left.x() + bottom_right.x()) / 2;
    float center_y = (bottom_right.y() + top_right.y()) / 2;

    QPointF center = {center_x, center_y};
    center = get_position(center);

    int distance = 30;
    {
        // Linea Horizontal
        QPointF horizontal_p1 = {center.x() - distance, center.y()};
        QPointF horizontal_p2 = {center.x() + distance, center.y()};

        horizontal_p1 = get_coordsf(horizontal_p1);
        horizontal_p2 = get_coordsf(horizontal_p2);

        draw_line(horizontal_p1, horizontal_p2, color);
        //
    }

    {
        // Linea Vertical
        QPointF vertical_p1 = {center.x(), center.y() - distance};
        QPointF vertical_p2 = {center.x(), center.y() + distance};

        vertical_p1 = get_coordsf(vertical_p1);
        vertical_p2 = get_coordsf(vertical_p2);

        draw_line(vertical_p1, vertical_p2, color);
        //
    }

    //
    //
}
