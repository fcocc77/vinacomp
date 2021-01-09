#include <viewer_gl.h>

viewer_gl::viewer_gl()
	: gl_view(true),
	fitted(true)
{
    center_viewer = new action("Center Image", "F", "center");
	center_viewer->connect_to(this, [=]() { fit_to_viewport(); });
}

viewer_gl::~viewer_gl()
{
}

void viewer_gl::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
}

void viewer_gl::resizeGL(int w, int h)
{
	if (fitted)
		fit_to_viewport();
}

void viewer_gl::fit_to_viewport()
{
	fitted = true;
	int margin = 30;
	int _width = 1920;
	int _height = 1080;

	float viewport_aspect = get_aspect();
	float resolution_aspect = float(_height) / _width;

	// primero lo ajusta a los bordes con ortho, y luego deja la escala simetrica
	// dejando la 2 escalas iguales x, y dependiendo si es ancho o alto
	set_ortho(-margin, _width + margin, -margin, _height + margin);

	if (viewport_aspect > resolution_aspect)
	{
		float scale_x = get_scale().x();
		set_scale({scale_x, scale_x});
	}
	else
	{
		float scale_y = get_scale().y();
		set_scale({scale_y, scale_y});
	}

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
	// si es zoom desabilita el ajuste para el cuadro de resolucion
    if (qt::alt() && event->button() == Qt::MidButton)
		fitted = false;

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
