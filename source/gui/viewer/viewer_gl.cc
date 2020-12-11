#include <viewer_gl.h>

viewer_gl::viewer_gl() : gl_view(true)
{
    center_viewer = new action("Center Image", "F", "center_a");
    center_viewer->connect_to(this, [=]() {
        int margin = 100;

		// primero ajusta el ortho en el alto, luego al obtener la escala
		// y la translacion en y se puede ajustar en ancho, esto se hace para
		// mantener el aspecto correcto.
        set_ortho(0, 0, -margin, 1080 + margin);
		float scale_y = get_scale().y();
		float translate_y = get_translate().y();
		set_transform({1920 / 2, translate_y}, {scale_y, scale_y});
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
