#include <time_line.h>

void time_line::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
}

void time_line::paintGL()
{
    gl_view::paintGL();

    draw_coordinate_numbers();
    draw_cursor();
}

void time_line::draw_cursor()
{
    float y1 = get_coords({0, 0}).y();
    float y2 = get_coords({0, height()}).y();

    draw_line({ghost_frame, y1}, {ghost_frame, y2}, QColor{150, 0, 0});
    draw_line({frame, y1}, {frame, y2}, Qt::red);

    glBegin(GL_TRIANGLES);
    glColor3f(0, 1, 0);

    glVertex2f(ghost_frame - 2, y1);
    glVertex2f(ghost_frame + 2, y1);
    glVertex2f(ghost_frame, y2);
    glVertex2f(ghost_frame - 2, y1);
    
    glEnd();
}

void time_line::draw_coordinate_numbers()
{
    auto horizontal_numbers = [=](float separation) {
        QColor color = QColor(200, 0, 0);
        for (float value : generate_coord_range(separation, Qt::Horizontal, color, {0.0, 10}))
            draw_text(QString::number(value), color, {value, 0}, {-1, height() - 20});
    };

    QList<float> time_separations = {1, 5, 10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000};
    for (float separation : time_separations)
        horizontal_numbers(separation);
}
