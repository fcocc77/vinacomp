#include <time_line.h>

time_line::time_line()
    : frame(10),
      ghost_frame(10),
      dragging(false),
      first_frame(1),
      last_frame(100),
      input(10),
      output(40),
      dragging_input(false),
      dragging_output(false),
      ghost_frame_visible(true),
      right_button(false),
      middle_button(false)

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

void time_line::drag_in_out(int _frame)
{
    if (_frame == input || _frame == output)
    {
        this->setCursor(Qt::SizeHorCursor);
        ghost_frame_visible = false;
    }
    else if (!dragging_input && !dragging_output)
    {
        this->setCursor(Qt::ArrowCursor);
        ghost_frame_visible = true;
    }

    if (dragging_input)
        input = _frame;
    else if (dragging_output)
        output = _frame;

    if (dragging_input || dragging_output)
        update();
}

void time_line::change_frame(int _frame)
{
    if (middle_button || right_button)
        return;

    // actualiza solo si es que el frame cambio
    if (_frame != ghost_frame)
    {
        ghost_frame = _frame;
        if (dragging && (!dragging_input && !dragging_output))
            frame = _frame;

        update();
    }
}

void time_line::mousePressEvent(QMouseEvent *event)
{
    dragging = true;

    right_button = event->button() == Qt::RightButton;
    middle_button = event->button() == Qt::MiddleButton;

    int x_pos = round(get_coords(event->pos()).x());

    dragging_input = x_pos == input;
    dragging_output = x_pos == output;

    change_frame(x_pos);

    update();

    gl_view::mousePressEvent(event);
}

void time_line::mouseReleaseEvent(QMouseEvent *event)
{

    dragging = false;
    dragging_input = false;
    dragging_output = false;
    right_button = false;
    middle_button = false;

    gl_view::mouseReleaseEvent(event);
}

void time_line::cursor_move_event(QPoint position)
{

    if (!qt::alt())
    {
        int x_pos = round(get_coords(position).x());
        drag_in_out(x_pos);
        change_frame(x_pos);
    }

    gl_view::cursor_move_event(position);
}