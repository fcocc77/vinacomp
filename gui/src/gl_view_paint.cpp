#include <gl_view.hpp>

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
    int gl_version = *glGetString(GL_VERSION);

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

        // mantiene un diferente tamaño de borde si es una tarjeta Nvidia o una intel
        if (gl_version == 52)
            glLineWidth(1.5); // Nvidia
        else
            glLineWidth(2); // Intel
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

void gl_view::draw_line(QPointF src, QPointF dst, QColor color)
{
    glBegin(GL_LINES);
    glColor4f(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0, 1);
    glVertex2f(src.x(), src.y());
    glVertex2f(dst.x(), dst.y());
    glEnd();
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

void gl_view::draw_text(QString text, QColor color, QPointF coords, QPointF viewer_position)
{
    int font_size = 10;

    int size_x = 50;
    int size_y = 10;

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

    painter.drawText(x - (size_x / 2), y - (size_y / 2), size_x, size_y, Qt::AlignCenter, text);
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
