#include <viewer_gl.h>

viewer_gl::viewer_gl()
	: gl_view(true)
	, fitted(true)
	, image(nullptr)
	, image_width(1920)
	, image_height(1080)
{
    center_viewer = new action("Center Image", "F", "center");
	center_viewer->connect_to(this, [=]() { fit_to_viewport(); });

    fit_100_percent = new action("Fit 100%", "Ctrl+1", "fit_100");
	fit_100_percent->connect_to(this, [=]() { fit_to_percent(100); });
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

void viewer_gl::paintGL()
{
    gl_view::paintGL();
	draw_image();
	draw_frame();
}

void viewer_gl::fit_to_viewport()
{
	fitted = true;
	int margin = 30;
	margin = ( image_height * margin ) / 1080; // Mantiene el margen equivalente al formato

	float viewport_aspect = get_aspect();
	float resolution_aspect = float(image_height) / image_width;

	// primero lo ajusta a los bordes con ortho, y luego deja la escala simetrica
	// dejando la 2 escalas iguales x, y dependiendo si es ancho o alto
	set_ortho(-margin, image_width + margin, -margin, image_height + margin);

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

void viewer_gl::fit_to_percent(int percent)
{
	fitted = false;

	// al dividir el ancho del viewport por 4, obtenemos la escala del 100% de la imagen
	int scale_100 = this->width() / 4;
	int scale = scale_100 * 100 / percent;
	set_scale({scale, scale});
	//

	update();
}

void viewer_gl::set_image(QImage *_image, int _image_width, int _image_height)
{
	image = _image;
	image_width = image->width();
	image_height = image->height();
	update();
}

void viewer_gl::draw_frame()
{
	QColor color = Qt::darkGray;

    draw_line({0, 0}, {0, image_height}, color);
    draw_line({0, image_height}, {image_width, image_height}, color);
    draw_line({image_width, image_height}, {image_width, 0}, color);
    draw_line({image_width, 0}, {0, 0}, color);

	QString format_label = QString::number(image_width) + " x " + QString::number(image_height);
	draw_text(format_label, color, {image_width, image_height}, {-1, -1}, 9, Qt::AlignRight, {5, -10});
}

void viewer_gl::draw_image()
{
	if (!image)
		return;

	// genera la textura 2d a partir de los bits de la imagen
	GLuint texture; 
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image_width, image_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, image->bits());
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
	glTexCoord2f(1.0f, 0.0f); glVertex2f(image_width, 0); // Inferior Derecha
	glTexCoord2f(1.0f, 1.0f); glVertex2f(image_width, image_height); // Superior Derecha
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0, image_height); // Superior Izquierda
	glEnd();
	glDisable(GL_TEXTURE_2D);
	//
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
