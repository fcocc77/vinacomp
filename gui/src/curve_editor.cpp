#include <curve_editor.hpp>

curve_editor ::curve_editor()
{
    this->setObjectName("curve_editor");
    setup_ui();

    qt::shortcut("F", this, [this]() {
        view->set_default();
    });
}

curve_editor ::~curve_editor()
{
}

void curve_editor::setup_ui()
{
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);

    QTreeWidget *knobs_tree = knobs_tree_setup_ui();

    layout->addWidget(knobs_tree);

    view = new curve_view();
    view->setObjectName("graphics_view");
    layout->addWidget(view);
    //
    //
}

QTreeWidget *curve_editor::knobs_tree_setup_ui()
{
    QTreeWidget *tree = new QTreeWidget();
    tree->setObjectName("knobs_tree");
    tree->setMaximumWidth(300);
    tree->setMinimumWidth(300);

    //
    //

    return tree;
}

curve_view::curve_view(/* args */)
{
    point = {-0.3, 0.3};
}

curve_view::~curve_view()
{
}

void curve_view::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);

    // glEnable(GL_DEPTH_TEST);
}

void curve_view::draw_line(QPointF src, QPointF dst, QColor color)
{
    glBegin(GL_LINES);
    glColor4f(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0, 0.0);
    glVertex2f(src.x(), src.y());
    glVertex2f(dst.x(), dst.y());
    glEnd();
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
    auto color_from_level = [](float a, float b, float value, QColor color) {
        float level = 1.0 - (1.0 * (value - a) / abs(a - b));

        int red = color.red() * level;
        int green = color.green() * level;
        int blue = color.blue() * level;

        return QColor(red, green, blue);
    };

    int out_frame = 100;
    QPointF top_left_point = map_position({-out_frame, -out_frame});
    QPointF down_right_point = map_position({width() + out_frame, height() + out_frame});

    float up_limit = top_left_point.y();
    float left_limit = top_left_point.x();

    float down_limit = down_right_point.y();
    float right_limit = down_right_point.x();

    float life_start = 0.5;
    float life_end = 70;

    auto horizontal_lines = [=](float separation, QColor color) {
        float scale = get_scale().y() / separation;

        if (scale < life_end && scale > life_start)
        {
            float range = (abs(down_limit - up_limit) / separation) * separation;
            float start = down_limit;
            float end = range + start;

            start /= separation;
            end /= separation;

            QColor _color = color_from_level(life_start, life_end, scale, color);
            for (int i = start; i < end; i++)
            {
                float value = i * separation;
                draw_line({left_limit, value}, {right_limit, value}, _color);
            }
        }
    };

    auto vertical_lines = [=](float separation, QColor color) {
        float scale = get_scale().x() / separation;

        if (scale < life_end && scale > life_start)
        {
            float range = (abs(left_limit - right_limit) / separation) * separation;
            float start = left_limit;
            float end = range + start;

            start /= separation;
            end /= separation;

            QColor _color = color_from_level(life_start, life_end, scale, color);
            for (int i = start; i < end; i++)
            {
                float value = i * separation;
                draw_line({value, down_limit}, {value, up_limit}, _color);
            }
        }
    };

    glEnable(GL_DEPTH_TEST);

    // Eje X e Y
    draw_line({left_limit, 0.0}, {right_limit, 0.0}, QColor(120, 30, 30));
    draw_line({0.0, down_limit}, {0.0, up_limit}, QColor(20, 120, 20));
    //
    //

    QList<float> separations = {100000, 10000, 1000, 100, 10, 1, 0.1, 0.01};
    QList<float> separations2 = {50000, 5000, 500, 50, 5, 0.5, 0.05, 0.005};

    for (float separation : separations)
    {
        horizontal_lines(separation, QColor(0, 55, 10));
        vertical_lines(separation, QColor(0, 55, 10));
    }

    for (float separation : separations2)
    {
        horizontal_lines(separation, QColor(0, 40, 10));
        vertical_lines(separation, QColor(0, 40, 10));
    }

    // float min = 0.01;
    // float max = 100000;

    // for (float i = max; i > (min - 0.001); i /= 10)
    // {
    //     horizontal_lines(i, QColor(0, 100, 20));
    //     vertical_lines(i, QColor(0, 100, 20));
    // }

    // for (float i = max; i > (min - 0.001); i /= 3)
    // {
    //     horizontal_lines(i, QColor(0, 70, 10));
    //     vertical_lines(i, QColor(0, 70, 10));
    // }

    // for (float i = max; i > (min - 0.001); i /= 2)
    // {
    //     horizontal_lines(i, QColor(0, 50, 7));
    //     vertical_lines(i, QColor(0, 50, 7));
    // }

    //
    //
}

void curve_view::paintGL()
{

    gl_view::paintGL();

    draw_grid();

    // glClear(GL_COLOR_BUFFER_BIT);
    QPainter painter(this);
    painter.setPen(Qt::red);
    painter.setFont(QFont("Arial", 16));

    QPointF position = get_position({10, 10});

    painter.drawText(position.x(), position.y(), 100, 30, Qt::AlignCenter, "Hello World!");
    painter.end();

    draw_line({0, 0}, {10, 10}, QColor(0, 0, 255));

    // draw_circle();
}

void curve_view::mousePressEvent(QMouseEvent *event)
{

    QPointF position = map_position(event->pos());

    point = {position.x(), position.y()};

    update();

    gl_view::mousePressEvent(event);
}