#include <viewer_gl.h>

viewer_gl::viewer_gl() : gl_view(true)
{
    center_viewer = new action("Center Image", "F", "center_a");
    center_viewer->connect_to(this, [=]() {
        int margin = 100;
        int width = 1920.0 / ((1920.0 / 1080.0) + get_aspect()) / 2;
        set_ortho(-margin, width + margin, -margin, 1080 + margin);
    });
}

viewer_gl::~viewer_gl()
{
}

void viewer_gl::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
}

void viewer_gl::draw_frame(int width, int height, QColor color)
{

    draw_line({0, 0}, {0, height}, color);
    draw_line({0, height}, {width, height}, color);
    draw_line({width, height}, {width, 0}, color);
    draw_line({width, 0}, {0, 0}, color);
}

void viewer_gl::paintGL()
{
    gl_view::paintGL();

    draw_frame(1920, 1080, Qt::darkGray);
}

void viewer_gl::mousePressEvent(QMouseEvent *event)
{

    gl_view::mousePressEvent(event);
}

void viewer_gl::mouseReleaseEvent(QMouseEvent *event)
{

    gl_view::mouseReleaseEvent(event);
}

void viewer_gl::mouseMoveEvent(QMouseEvent *event)
{

    gl_view::mouseMoveEvent(event);
}