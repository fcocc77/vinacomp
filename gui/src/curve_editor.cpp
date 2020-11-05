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

    QSurfaceFormat format;
    format.setSamples(10);
    setFormat(format);
}

void curve_view::paintGL()
{
    glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex2f(0.0f, 1.0f);
    glVertex2f(1.0f, 0.0f);
    glEnd();
}

void curve_view::resizeGL(int w, int h)
{

    glViewport(0, 0, w, h);
}