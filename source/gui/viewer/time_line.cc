#include <time_line.h>

time_line::time_line()
    : frame(10),
      ghost_frame(10),
      dragging(false),
      first_frame(1),
      last_frame(100),
      input(3),
      output(40),
      click_input(0),
      click_output(0),
      dragging_input(false),
      dragging_output(false),
      ghost_frame_visible(true),
      right_button(false),
      middle_button(false)

{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setMinimumHeight(50);

    fit_to_range();

    action *fit_to_range_action = new action("Fit to Range", "F");
    fit_to_range_action->connect_to(this, [this]() {
        fit_to_range();
    });

    action *input_action = new action("Input", "I");
    input_action->connect_to(this, [this]() {
        set_in_out(frame, output);
        update();
    });

    action *ouput_action = new action("Input", "O");
    ouput_action->connect_to(this, [this]() {
        set_in_out(input, frame);
        update();
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
    {
        if (qt::control() && click_over_in_out.first)
            set_in_out(_frame, (click_output + _frame - click_x_coords));
        else
            set_in_out(_frame, output);
    }
    else if (dragging_output)
        if (qt::control() && click_over_in_out.second)
            set_in_out((click_input + _frame - click_x_coords), _frame);
        else
            set_in_out(input, _frame);

    if (dragging_input || dragging_output)
        update();
}

void time_line::set_in_out(int _input, int _output)
{
    // si el input o output sobrepasa al otro, los limita con 1 frame menos
    if (_input >= output)
        _input = output - 1;

    if (_output <= input)
        _output = input + 1;
    //
    //

    // si el frame esta fuera del rango, lo actualiza y retorna
    if (_input < first_frame)
        _input = first_frame;
    else if (_output > last_frame)
        _output = last_frame;
    //
    //

    output = _output;
    input = _input;
}

void time_line::change_in_out_with_control()
{
    if (!qt::control())
        return;

    int input_distance = output - frame;
    int output_distance = frame - input;

    int aux_input = input,
        aux_output = output;

    // si el click sobrepasa el frame hacia a izquierda, actua sobre el 'input',
    // si sobrepasa hacia la derecha actua sobre el 'output', y con otras condicionales
    // para definir cuando se arrastraran input y output a la vez
    if (click_x_coords > frame)
    {
        if (input < frame || dragging_output)
        {
            aux_output = click_x_coords;
            dragging_output = true;
        }
        else
        {
            aux_input = click_x_coords;
            dragging_input = true;
        }
    }
    else
    {
        if (output > frame || dragging_input)
        {
            aux_input = click_x_coords;
            dragging_input = true;
        }
        else
        {
            aux_output = click_x_coords;
            dragging_output = true;
        }
    }
    //
    //

    set_in_out(aux_input, aux_output);
}

void time_line::set_frame(int _frame)
{
    if (middle_button || right_button || qt::control())
        return;

    // solo actualiza cuando cambia el frame y asi ahorramos recursos
    if (_frame != ghost_frame)
    {
        ghost_frame = _frame;
        update();
    }
    if (_frame != frame)
        if (dragging && (!dragging_input && !dragging_output))
        {
            frame = _frame;
            update();
        }
}

void time_line::mousePressEvent(QMouseEvent *event)
{
    click_x_coords = round(get_coords(event->pos()).x());
    dragging = true;

    click_input = input;
    click_output = output;
    click_over_in_out = over_in_out(click_x_coords);

    right_button = event->button() == Qt::RightButton;
    middle_button = event->button() == Qt::MiddleButton;

    if (right_button)
        fit_to_range();

    dragging_input = click_over_in_out.first;
    dragging_output = click_over_in_out.second;

    change_in_out_with_control();

    if (!qt::alt() && !qt::control)
        set_frame(click_x_coords);

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
        set_frame(x_pos);
    }

    gl_view::cursor_move_event(position);
}

void time_line::focusOutEvent(QFocusEvent *event)
{
	ghost_frame_visible = false;
	update();
}
