#include <viewer_gl.h>

viewer_gl::viewer_gl(render_data *_rdata)
	: rdata(_rdata)
	, gl_view(true)
	, fitted(true)
	, overlay(true)
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

	if (overlay)
	{
		draw_bbox();
		draw_frame();

		box_handler_draw();
	}

}

void viewer_gl::fit_to_viewport()
{
	fitted = true;
	int margin = 30;
	margin = ( rdata->height * margin ) / 1080; // Mantiene el margen equivalente al formato

	float viewport_aspect = get_aspect();
	float resolution_aspect = float(rdata->height) / rdata->width;

	// primero lo ajusta a los bordes con ortho, y luego deja la escala simetrica
	// dejando la 2 escalas iguales x, y dependiendo si es ancho o alto
	set_ortho(-margin, rdata->width + margin, -margin, rdata->height + margin);

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

void viewer_gl::isolate_channel(int channel)
{
	GLint ch;
	if (channel == -1)
		ch = GL_RGB8;
	else if (channel == 0)
		ch = GL_RED;
	else if (channel == 1)
		ch = GL_GREEN;
	else if (channel == 2)
		ch = GL_BLUE;

	// deja cada canal en su lugar, antes de isolar alguno si es que lo necesita
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_BLUE);

	if (channel != -1)
	{
		if (channel != 0)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, ch);
		if (channel != 1)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, ch);
		if (channel != 2)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, ch);
	}
}

void viewer_gl::draw_image()
{
	GLuint texture;
	glBindTexture(GL_TEXTURE_2D, texture);

	// genera la textura 2d a partir de los bits de la imagen
	rdata->image.convertTo(rdata->image, CV_8U);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, rdata->image.cols, rdata->image.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, rdata->image.data);

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

	int x = rdata->bbox.x();
	int y = rdata->bbox.y();
	// if (x < 0) x = 0;
	// if (y < 0) y = 0;

	glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y + rdata->image.rows); // Superior Izquierda
	glTexCoord2f(1.0f, 0.0f); glVertex2f(x + rdata->image.cols, y + rdata->image.rows); // Superior Derecha;
	glTexCoord2f(1.0f, 1.0f); glVertex2f(x + rdata->image.cols, y); // Inferior Derecha
	glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y); // Inferior Izquierda
	glEnd();
	glDisable(GL_TEXTURE_2D);
	//
}

void viewer_gl::draw_bbox()
{
	QColor color = Qt::darkGray;
	int stipple = 2;

	QRect &bb = rdata->bbox;
	QPoint top_left = { bb.x(), bb.y() + bb.height() };
	QPoint top_right = { bb.x() + bb.width(), bb.y() + bb.height() };
	QPoint bottom_right = { bb.x() + bb.width(), bb.y() };
	QPoint bottom_left = { bb.x(), bb.y()};

	draw_dashed_line({ bottom_left, top_left }, color, stipple);
	draw_dashed_line({ bottom_left, bottom_right }, color, stipple);
	draw_dashed_line({ bottom_right, top_right }, color, stipple);
	draw_dashed_line({ top_right, top_left }, color, stipple);
}

void viewer_gl::draw_frame()
{
	QColor color = Qt::darkGray;

    draw_line({0, 0}, {0, rdata->height}, color);
    draw_line({0, rdata->height}, {rdata->width, rdata->height}, color);
    draw_line({rdata->width, rdata->height}, {rdata->width, 0}, color);
    draw_line({rdata->width, 0}, {0, 0}, color);

	QString format_label = QString::number(rdata->width) + " x " + QString::number(rdata->height);
	draw_text(format_label, color, {rdata->width, rdata->height}, {-1, -1}, 9, Qt::AlignRight, {5, -10});
}

void viewer_gl::set_overlay(bool _overlay)
{
	overlay = _overlay;
	update();
}

void viewer_gl::mousePressEvent(QMouseEvent *event)
{
	// si es zoom desabilita el ajuste para el cuadro de resolucion
    if (qt::alt() && event->button() == Qt::MidButton)
		fitted = false;

	if (event->button() == Qt::RightButton)
		right_click();

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
