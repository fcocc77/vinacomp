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
    float cx = 0;
    float cy = 0;
    float r = 0.7;
    int num_segments = 100;

    glBegin(GL_LINE_LOOP);

    glColor4f(1, 0, 0, 0);

    for (int ii = 0; ii < num_segments; ii++)
    {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments); //get the current angle
        float x = r * cosf(theta);                                         //calculate the x component
        float y = r * sinf(theta);                                         //calculate the y component
        glVertex2f(x + cx, y + cy);                                        //output vertex
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

void curve_view::draw_point(QPointF coord)
{
    glEnable(GL_POINT_SMOOTH);
    glPointSize(7);
    glBegin(GL_POINTS);
    glColor3f(0, 0, 1);
    glVertex2f(coord.x(), coord.y());
    glEnd();
}

void curve_view::create_handle(QPoint position)
{

    QPointF coord = get_coords(position);
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
                draw_bezier(last_key.pos, key.pos);

            last_key = key;
            is_first = false;
        }

        //
        //

        // Interpolations
        float separation = 0.2;
        for (key_frame key : curve)
        {
            float point_a_x = key.pos.x() - separation;
            float point_b_x = key.pos.x() + separation;

            QPointF point_a = {point_a_x, key.pos.y()};
            QPointF point_b = {point_b_x, key.pos.y()};
            draw_line(point_a, point_b, Qt::red);
            draw_point(point_a);
            draw_point(point_b);
        }
        //

        // Points
        for (key_frame key : curve)
            draw_point(key.pos);
        //
    }
}

QPointF curve_view::cubic_bezier(QPointF point_a, QPointF point_b, float value, float exaggeration)
{
    float distance = abs(point_b.x() - point_a.x()) * exaggeration;

    QPointF point_a2 = {point_a.x() + distance, point_a.y()};
    QPointF point_b2 = {point_b.x() - distance, point_b.y()};

    // Algoritmo bezier
    QPointF L1 = ((1 - value) * point_a) + (value * point_a2);
    QPointF L2 = ((1 - value) * point_a2) + (value * point_b2);
    QPointF L3 = ((1 - value) * point_b2) + (value * point_b);

    QPointF Q1 = ((1 - value) * L1) + (L2 * value);
    QPointF Q2 = ((1 - value) * L2) + (L3 * value);

    return ((1 - value) * Q1) + (Q2 * value);
    //
}

void curve_view::draw_bezier(QPointF src_key, QPointF dst_key)
{
    glBegin(GL_LINE_STRIP);
    glColor3f(1, 1, 0);

    int segments = 100;
    float segment = 1.0 / segments;
    float t = 0;

    for (int i = 0; i <= segments; i++)
    {
        QPointF point = cubic_bezier(src_key, dst_key, t);
        glVertex2f(point.x(), point.y());

        t += segment;
    }

    glEnd();
}

void curve_view::create_curve()
{

    key_frame key1 = {{0.1, 0.2}, 0, 0};
    key_frame key2 = {{0.5, 1}, 0, 0};
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

void curve_view::mousePressEvent(QMouseEvent *event)
{
    int draw_tolerance = 10; // Pixels
    for (auto curve : curves.keys())
    {
        auto keys = curves[curve];
        for (int i = 0; i < keys.count(); i++)
        {
            QPointF key = keys[i].pos;
            QPointF key_position = get_position(key);
            float distance = qt::distance_points(key_position, event->pos());
            if (distance < draw_tolerance)
            {
                drag_curve = curve;
                drag_index = i;
                is_drag = true;

                break;
            }
        }
    }

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
                curves[drag_curve][drag_index].pos = get_coords(event->pos());
                update();
            }
        }
    }

    gl_view::mouseMoveEvent(event);
}