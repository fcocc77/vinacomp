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

	image = QImage("/home/pancho/Desktop/coffe.jpg");
	image = image.mirrored();
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

void viewer_gl::draw_image()
{
	int width = image.width();
	int height = image.height();

	// genera la textura 2d a partir de los bits de la imagen
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, image.constBits());
	glGenerateMipmap(GL_TEXTURE_2D);
	//

	// si el zoom es menor a 100, muestra los pixels en la imagen
	if (get_scale().x() < 100)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	//

	// genera un rectangulo con las cordenadas de la textura
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0); // Inferior Izquierda
	glTexCoord2f(1.0f, 0.0f); glVertex2f(width, 0); // Inferior Derecha
	glTexCoord2f(1.0f, 1.0f); glVertex2f(width, height); // Superior Derecha
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0, height); // Superior Izquierda
	glEnd();
	glDisable(GL_TEXTURE_2D);
	//
}

void viewer_gl::paintGL()
{
    gl_view::paintGL();
	draw_image();
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
