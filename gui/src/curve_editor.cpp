#include <curve_editor.hpp>

curve_editor ::curve_editor(/* args */)
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
    glClearColor(0, .1, 0, 1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);
    glShadeModel(GL_SMOOTH);

    format().setSamples(10);
}

void curve_view::paintGL()
{
    // Eje X
    glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glEnd();
    //
    //

    // Eje Y
    glBegin(GL_LINES);
    glColor3f(0, 1, 0);
    glVertex2f(0.0f, -1.0f);
    glVertex2f(0.0f, 1.0f);
    glEnd();
    //
    //

    glBegin(GL_LINE_STRIP);
    glColor3f(0, 0, 1);
    glVertex2f(0.5, -0.2);
    glVertex2f(-0.2, 0.5);
    glVertex2f(point.x(), point.y());
    glEnd();

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

void curve_view::resizeGL(int w, int h)
{

    glViewport(0, 0, w, h);
}

QPointF curve_view::map_position(QPoint mouse_position)
{
    float zoom_val = 1.0;
    QList<float> ortho_2d = {-zoom_val, +zoom_val, -zoom_val, +zoom_val};

    // Primero, calcular las coordenadas "normalizadas" del mouse dividiendo por tamaño
    float mouse_norm_x = float(mouse_position.x()) / size().width();
    float mouse_norm_y = float(mouse_position.y()) / size().height();

    // Mapear coordenadas al rango de proyección ortográfica
    float mouse_ortho_x = (mouse_norm_x * (ortho_2d[1] - ortho_2d[0])) + ortho_2d[0];
    float mouse_ortho_y = (mouse_norm_y * (ortho_2d[3] - ortho_2d[2])) + ortho_2d[2];

    return {mouse_ortho_x, -mouse_ortho_y};
}

void curve_view::mousePressEvent(QMouseEvent *event)
{

    QPointF position = map_position(event->pos());

    point = {position.x(), position.y()};

    update();
}