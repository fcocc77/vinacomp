#include <time_line.h>

time_line::time_line()
    : frame(10),
      ghost_frame(10),
      dragging(false),
      first_frame(1),
      last_frame(100)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setMinimumHeight(50);

    action *fit_to_range = new action("Fit to Range", "F");
    fit_to_range->connect_to(this, [this]() {
        int padding = 5;
        set_ortho(first_frame - padding, last_frame + padding, 0, 1);
    });
}

time_line::~time_line()
{
}

void time_line::mousePressEvent(QMouseEvent *event)
{
    dragging = true;

    frame = round(get_coords(event->pos()).x());
    update();

    gl_view::mousePressEvent(event);
}

void time_line::mouseReleaseEvent(QMouseEvent *event)
{

    dragging = false;

    gl_view::mouseReleaseEvent(event);
}

void time_line::cursor_move_event(QPoint position)
{
    if (!qt::alt())
    {
        int x_pos = round(get_coords(position).x());
        // actualiza solo si es que el frame cambio
        if (x_pos != ghost_frame)
        {
            ghost_frame = x_pos;
            if (dragging)
                frame = x_pos;

            update();
        }
    }

    gl_view::cursor_move_event(position);
}