#include <gl_view.hpp>

void gl_view::paintGL()
{
    // zoom y paneo
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = float(height()) / width();

    float left = coord.x() - zoom_scale.x() * 2;
    float right = coord.x() + zoom_scale.x() * 2;
    float top = (coord.y() * aspect) + (zoom_scale.y() * 2) * aspect;
    float bottom = (coord.y() * aspect) - (zoom_scale.y() * 2) * aspect;

    glOrtho(left, right, bottom, top, -1.f, 1.f);

    glMatrixMode(GL_MODELVIEW);
    //
    //
}

void gl_view::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}

void gl_view::draw_point(QPointF coord, QColor color, int size)
{
    glEnable(GL_POINT_SMOOTH);
    glPointSize(size);
    glBegin(GL_POINTS);
    glColor3f(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0);
    glVertex2f(coord.x(), coord.y());
    glEnd();
}

void gl_view::draw_line(QPointF src, QPointF dst, QColor color)
{
    glBegin(GL_LINES);
    glColor4f(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0, 1);
    glVertex2f(src.x(), src.y());
    glVertex2f(dst.x(), dst.y());
    glEnd();
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
