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
        glBegin(GL_LINE_STRIP);
        glColor3f(0, 0, 1);

        glVertex2f(-1000000, curve.first().y());

        for (QPointF key : curve)
            glVertex2f(key.x(), key.y());

        glVertex2f(1000000, curve.last().y());

        glEnd();

        for (QPointF key : curve)
            draw_point(key);
    }
}

void curve_view::create_curve()
{
    QPointF key1 = {0.1, 0.2};
    QPointF key2 = {0.5, 1};
    QPointF key3 = {1, 0.3};

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
            QPointF key = keys[i];
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
        if (curves.contains(drag_curve))
        {
            curves[drag_curve][drag_index] = get_coords(event->pos());
            update();
        }
    }

    gl_view::mouseMoveEvent(event);
}