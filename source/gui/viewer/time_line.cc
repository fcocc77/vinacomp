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
      middle_button(false),
	  selector_visible(false),
	  is_fit_to_selector(true)

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

void time_line::fit_switch()
{
	// conmutador entre el la ultima seleccion que se hizo,
	// y el ajuste al rango.
	if (is_fit_to_selector)
		fit_to_range();
	else
		fit_to_selector();
}

void time_line::fit_to_range()
{
	is_fit_to_selector = false; 
    int padding = 5;
    set_ortho(first_frame - padding, last_frame + padding, 0, 1);
}

void time_line::fit_to_selector()
{
	if (!selector.first && !selector.second)
		return;

	is_fit_to_selector = true; 
    set_ortho(selector.first, selector.second, 0, 1);
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

	if (!right_button)
	{
		dragging_input = click_over_in_out.first;
    	dragging_output = click_over_in_out.second;

    	change_in_out_with_control();
	}

    if (!qt::alt() && !qt::control)
        set_frame(click_x_coords);

    update();

    gl_view::mousePressEvent(event);
}

void time_line::mouseReleaseEvent(QMouseEvent *event)
{
    if (right_button)
	{	
		if (selector_visible)
			fit_to_selector();
		else
        	fit_switch();
	}

    dragging = false;
    dragging_input = false;
    dragging_output = false;
    right_button = false;
    middle_button = false;
    ghost_frame_visible = true;
	selector_visible = false;

	update();
    gl_view::mouseReleaseEvent(event);
}

void time_line::cursor_move_event(QPoint position)
{		
	int move_x_coords = round(get_coords(position).x());
	
	// Actualiza cuanto esta presionado el boton derecho, significa
	// que esta seleccionando.
	if (right_button)
	{
		if (click_x_coords != move_x_coords)
		{
			selector_visible = true;
			selector.first = click_x_coords;
			selector.second = move_x_coords;
			is_fit_to_selector = true;
		}
		else
			selector_visible = false;
		update();
	}
	//
	//

    if (!qt::alt())
    {
        drag_in_out(move_x_coords);
        set_frame(move_x_coords);
    }

    gl_view::cursor_move_event(position);
}

void time_line::focusOutEvent(QFocusEvent *event)
{
	ghost_frame_visible = false;
	update();
}
