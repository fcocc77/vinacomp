#include <gl_view.h>

void gl_view::paintGL()
{
    // zoom y paneo
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = get_aspect();

    float left = translate.x() - scale.x() * 2;
    float right = translate.x() + scale.x() * 2;
    float top = (translate.y() * aspect) + (scale.y() * 2) * aspect;
    float bottom = (translate.y() * aspect) - (scale.y() * 2) * aspect;

    glOrtho(left, right, bottom, top, -1.f, 1.f);

    glMatrixMode(GL_MODELVIEW);
    //
    //
}

void gl_view::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}

void gl_view::aa_enable(bool enable)
{
    // habilita o desabilita el Antialiasing, si esta habilitado
    // usa la mescla en alpha y si no, usa una mescla
    // dejando los pixel mas claros visibles.
    if (enable)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

        glBlendEquation(GL_FUNC_ADD);

        glLineWidth(1.5);
    }
    else
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ZERO);

        // Siempre deja la luz mas clara
        glBlendEquation(GL_MAX);
        //

        glDisable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

        glLineWidth(1);
    }
}

void gl_view::draw_point(QPointF coord, QColor color, int size, bool smooth)
{
    if (smooth)
        glEnable(GL_POINT_SMOOTH);
    glPointSize(size);
    glBegin(GL_POINTS);
    glColor3f(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0);
    glVertex2f(coord.x(), coord.y());
    glEnd();

    if (smooth)
        glDisable(GL_POINT_SMOOTH);
}

void gl_view::draw_line(QPointF src, QPointF dst, QColor color, float width)
{
    glLineWidth(width);
    glBegin(GL_LINES);
    glColor4f(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0, 1);
    glVertex2f(src.x(), src.y());
    glVertex2f(dst.x(), dst.y());
    glEnd();
    glLineWidth(1);
}

void gl_view::draw_dashed_line(QLineF line, QColor color, int stipple)
{
    glPushAttrib(GL_ENABLE_BIT);

    glLineStipple(stipple, 0xAAAA);
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glColor3f(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0);

    glVertex2f(line.x1(), line.y1());
    glVertex2f(line.x2(), line.y2());

    glEnd();

    glPopAttrib();
}

void gl_view::draw_text(
    QString text,
    QColor color,
    QPointF coords,
    QPointF viewer_position,
    int font_size,
    Qt::Alignment align,
    QPointF offset)
{
    QPainter painter(this);
    painter.setPen(color);
    painter.setFont(QFont("Arial", font_size));

    // posicion en el visor
    QPointF position_from_coords = get_position(coords);
    //

    // Determina la posicion si es desde el visor o desde las cordenadas
    float x, y;
    if (viewer_position.x() >= 0)
        x = viewer_position.x();
    else
        x = position_from_coords.x();

    if (viewer_position.y() >= 0)
        y = viewer_position.y();
    else
        y = position_from_coords.y();
    //
    //

    QSize size = QFontMetrics(painter.font()).size(Qt::TextSingleLine, text);

    if (align == Qt::AlignLeft)
        x -= size.width();
    else if (align == Qt::AlignCenter)
        x -= size.width() / 2;

    y -= size.height() / 2;

    x += offset.x();
    y -= offset.y();

	aa_enable(false);
    painter.drawText(x, y, size.width(), size.height(), Qt::AlignCenter, text);

    painter.end();
}

void gl_view::draw_box(QLineF diagonal_line, QColor color, QColor border_color)
{
    aa_enable(false);

    QPointF p1 = diagonal_line.p1();
    QPointF p2 = {diagonal_line.x1(), diagonal_line.y2()};
    QPointF p3 = diagonal_line.p2();
    QPointF p4 = {diagonal_line.x2(), diagonal_line.y1()};

    draw_line(p1, p2, border_color);
    draw_line(p2, p3, border_color);
    draw_line(p3, p4, border_color);
    draw_line(p4, p1, border_color);

    glEnable(GL_BLEND);
    glColor3f(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0);
    glRectf(p1.x(), p1.y(), p3.x(), p3.y());
}

void gl_view::draw_triangle(QPointF pos, float size, QColor color, bool anchor_on_tip, float rotate)
{
    glBegin(GL_TRIANGLES);
    glColor3f(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0);

    QPointF anchor_point = get_position(pos);
	QPointF _position = anchor_point;

	if (anchor_on_tip)
		_position.setY(_position.y() - size + 1);

    QPointF point_1 = {_position.x() - size, _position.y()};
    QPointF point_2 = {_position.x() + size, _position.y()};
    QPointF point_3 = {_position.x(), _position.y() + size};

	if (rotate != 0)
	{
		point_1 = rotate_point(point_1, anchor_point, rotate, false, true);
		point_2 = rotate_point(point_2, anchor_point, rotate, false, true);
		point_3 = rotate_point(point_3, anchor_point, rotate, false, true);
	}

    point_1 = get_coordsf(point_1);
    point_2 = get_coordsf(point_2);
    point_3 = get_coordsf(point_3);

    glVertex2f(point_1.x(), point_1.y());
    glVertex2f(point_2.x(), point_2.y());
    glVertex2f(point_3.x(), point_3.y());

    glEnd();
}

void gl_view::draw_centered_box(QPointF pos, float size, QColor color, float rotate)
{
    QPointF _pos = get_position(pos);

	glBegin(GL_POLYGON);
    glColor3f(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0);

	size /= 2;

	int x1 = _pos.x() - size;
	int x2 = _pos.x() + size;
	int y1 = _pos.y() - size;
	int y2 = _pos.y() + size;

	QPointF p1 = {x1, y1};
	QPointF p2 = {x2, y1};
	QPointF p3 = {x2, y2};
	QPointF p4 = {x1, y2};

	if (rotate != 0)
	{
		p1 = rotate_point(p1, _pos, rotate, false, true);
		p2 = rotate_point(p2, _pos, rotate, false, true);
		p3 = rotate_point(p3, _pos, rotate, false, true);
		p4 = rotate_point(p4, _pos, rotate, false, true);
	}

	p1 = get_coordsf(p1);
	p2 = get_coordsf(p2);
	p3 = get_coordsf(p3);
	p4 = get_coordsf(p4);

	glVertex2f(p1.x(), p1.y());
	glVertex2f(p2.x(), p2.y());
	glVertex2f(p3.x(), p3.y());
	glVertex2f(p4.x(), p4.y());

	glEnd();
}

void gl_view::draw_circle(QPointF anchor_point, float ratio,
		QColor color, bool keep_scale, int num_segments, bool fill)
{
	if (keep_scale)
		ratio *= get_scale().x() / width();

	ratio /= 2;

	if (fill)
		glBegin(GL_POLYGON);
	else
		glBegin(GL_LINE_STRIP);

    glColor3f(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0);


    float segment = 360.0 / num_segments;
    float angle = 0;
    for (int i = 0; i <= num_segments; i++)
    {
		QPointF point = rotate_point({anchor_point.x() + ratio, anchor_point.y()}, anchor_point, angle);
        glVertex2f(point.x(), point.y());

        angle += segment;
    }
    glEnd();
}
