#include <time_line.h>

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
        }
    }
    //
    //

    set_in_out(aux_input, aux_output);
}

