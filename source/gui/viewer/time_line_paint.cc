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
    draw_in_out();
    draw_cursor();
}

void time_line::draw_in_out()
{
    QColor red = {200, 0, 0};
    draw_line({input, 1000000}, {input, -1000000}, red, 2);
    draw_line({output, 1000000}, {output, -1000000}, red, 2);

    float input_x = get_position({input, 0}).x();
    float output_x = get_position({output, 0}).x();

    draw_text(QString::number(input), red, {0, 0}, {input_x - 3, 7}, 10, Qt::AlignLeft);
    draw_text(QString::number(output), red, {0, 0}, {output_x + 5, 7}, 10, Qt::AlignRight);
}

void time_line::draw_cursor()
{
    float y1 = get_coords({0, 20}).y();
    float y2 = get_coords({0, height()}).y();
    float text_y = get_coords({0, 10}).y();

    if (ghost_frame_visible)
    {
        QColor ghost_frame_color = QColor{150, 100, 0};
        draw_line({ghost_frame, y1}, {ghost_frame, y2}, ghost_frame_color);
        draw_triangle({ghost_frame, y1}, 7, ghost_frame_color);
        draw_text(QString::number(ghost_frame), ghost_frame_color, {ghost_frame, text_y});
    }

    QColor frame_color = {255, 170, 0};
    draw_line({frame, y1}, {frame, y2}, frame_color);
    draw_triangle({frame, y1}, 7, frame_color);
    draw_text(QString::number(frame), frame_color, {frame, text_y});
}

void time_line::draw_coordinate_numbers()
{
    auto horizontal_numbers = [=](float separation) {
        QColor color = Qt::gray;
        for (float value : generate_coord_range(separation, Qt::Horizontal, color, {0.0, 10}))
            draw_text(QString::number(value), color, {value, 0}, {-1, height() - 20});
    };

    QList<float> time_separations = {1, 10, 100, 1000, 10000, 100000};
    for (float separation : time_separations)
        horizontal_numbers(separation);
}
