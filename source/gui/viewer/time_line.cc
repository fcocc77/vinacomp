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

    action *fit_to_range_action = new action("Fit to Range", "F");
    fit_to_range_action->connect_to(this, [this]() {
        fit_to_range();
    });
}

time_line::~time_line()
{
}

void time_line::fit_to_range()
{
    int padding = 5;
    set_ortho(first_frame - padding, last_frame + padding, 0, 1);
}

pair<bool, bool> time_line::over_in_out(int x) const
{
    int viewport_x = get_position({x, 0}).x();

    // verifica si el cursor del mouse esta sobre el input o el output
    QPointF viewport_input = get_position({input, 0});
    float input_distance = qt::distance_points({viewport_input.x(), 0}, {viewport_x, 0});

    QPointF viewport_output = get_position({output, 0});
    float output_distance = qt::distance_points({viewport_output.x(), 0}, {viewport_x, 0});

    int tolerance = 10;

    return {input_distance < tolerance, output_distance < tolerance};
}

void time_line::drag_in_out(int _frame)
{
    if (_frame < first_frame || _frame > last_frame)
        return;

    auto _over_in_out = over_in_out(_frame);
    if (_over_in_out.first || _over_in_out.second)
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

    if (right_button)
        fit_to_range();

    int x_pos = round(get_coords(event->pos()).x());

    auto _over_in_out = over_in_out(x_pos);
    dragging_input = _over_in_out.first;
    dragging_output = _over_in_out.second;

    if (qt::alt())
        ghost_frame_visible = false;
    else
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
    ghost_frame_visible = true;

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