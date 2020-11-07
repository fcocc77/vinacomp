#include <curve_editor.hpp>

curve_editor ::curve_editor()
{
    this->setObjectName("curve_editor");
    setup_ui();
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

    curve_view *view = new curve_view();
    view->setObjectName("graphics_view");
    layout->addWidget(view);

    // create a scene and add it your view
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setSceneRect(-500000, -500000, 1000000, 1000000);

    // view->setScene(scene);

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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_MODELVIEW);
    glShadeModel(GL_SMOOTH);

    format().setSamples(10);
}

void curve_view::draw_line(QPointF src, QPointF dst, QColor color)
{
    glBegin(GL_LINES);
    glColor3f(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0);
    glVertex2f(src.x(), src.y());
    glVertex2f(dst.x(), dst.y());
    glEnd();
}

void curve_view::draw_grid()
{

    QPointF up_a = map_position({10, 100});
    QPointF up_b = map_position({800, 100});
    draw_line(up_a, up_b, QColor(255, 0, 0));

    QPointF down_a = map_position({10, 400});
    QPointF down_b = map_position({800, 400});
    draw_line(down_a, down_b, QColor(255, 0, 0));
    //
    //
    //
    float scale_y = get_scale().y();

    auto rango_to_level = [](float a, float b, float value) {
        return 255 - (255.0 * (value - a) / abs(a - b));
    };

    float up_limit = up_a.y();
    float down_limit = down_a.y();

    auto grid = [=](float separation) {
        float scale = scale_y / separation;
        if (scale < 20 && scale > 1)
        {
            float caben = (abs(down_limit - up_limit) / separation) * separation;
            float start = down_limit;
            float end = caben + start;

            start /= separation;
            end /= separation;

            float level = rango_to_level(1, 20, scale);
            for (int i = start; i < end; i++)
            {
                float value = i * separation;
                draw_line({-2, value}, {2, value}, QColor(0, level, 0));
            }
        }
    };

    grid(125);
    grid(25);
    grid(5);
    grid(1);
}

void curve_view::paintGL()
{
    gl_view::paintGL();

    draw_grid();

    // Eje X
    // glBegin(GL_LINES);
    // glColor3f(1, 0, 0);
    // glVertex2f(-1.0f, 0.0f);
    // glVertex2f(1.0f, 0.0f);
    // glEnd();
    //
    //
    return;
    // Eje Y
    draw_line({0.0, -1.0}, {0.0, 1.0}, QColor(0, 255, 0));
    //
    //

    // glBegin(GL_LINE_STRIP);
    // glColor3f(0, 0, 1);
    // glVertex2f(0.5, 0.2);
    // glVertex2f(0.5, 0.5);
    // glVertex2f(point.x(), point.y());
    // glEnd();

    float cx = 0;
    float cy = 0;
    float r = 0.7;
    int num_segments = 100;

    glBegin(GL_LINE_LOOP);

    for (int ii = 0; ii < num_segments; ii++)
    {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments); //get the current angle
        float x = r * cosf(theta);                                         //calculate the x component
        float y = r * sinf(theta);                                         //calculate the y component
        glVertex2f(x + cx, y + cy);                                        //output vertex
    }
    glEnd();
}

void curve_view::mousePressEvent(QMouseEvent *event)
{

    QPointF position = map_position(event->pos());

    point = {position.x(), position.y()};

    update();

    gl_view::mousePressEvent(event);
}