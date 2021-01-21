#include <time_line.h>

time_line::time_line(
			QWidget *_parent,
			QLineEdit *_frame_edit,
			QLineEdit *_input_frame_edit,
			QLineEdit *_output_frame_edit,
			QLineEdit *_skip_frame_edit
		)

		: parent(_parent),
		frame_edit(_frame_edit),
		input_frame_edit(_input_frame_edit),
		output_frame_edit(_output_frame_edit),
		skip_frame_edit(_skip_frame_edit),

		frame(10),
		ghost_frame(10),
		dragging(false),
		first_frame(1),
		last_frame(100),
		input(1),
		output(100),
		click_input(0),
		click_output(0),
		dragging_input(false),
		dragging_output(false),
		ghost_frame_visible(true),
		right_button(false),
		middle_button(false),
		selector_visible(false),
		is_fit_to_selector(true),
		in_out_visible(false)

{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setMinimumHeight(50);

	palette = jread("stylesheet/palette.json");

    fit_to_range(first_frame, last_frame);

    action *fit_to_range_action = new action("Fit to Range", "F");
    fit_to_range_action->connect_to(this, [this]() {
        fit_to_range(first_frame, last_frame);
    });

    action *next_frame_action = new action("Next Frame", "right");
    next_frame_action->connect_to(parent, [this]()
	{
		next_frame();
    });

    action *previous_frame_action = new action("Previous Frame", "left");
    previous_frame_action->connect_to(parent, [this]()
	{
		previous_frame();
    });

    action *next_frame_each_action = new action("Skip Next Frames", "Ctrl+right");
    next_frame_each_action->connect_to(parent, [this]()
	{
		int skip_frames = skip_frame_edit->text().toInt();
		next_frame_each(skip_frames);
    });

    action *previous_frame_each_action = new action("Skip Previous Frames", "Ctrl+left");
    previous_frame_each_action->connect_to(parent, [this]()
	{
		int skip_frames = skip_frame_edit->text().toInt();
		previous_frame_each(skip_frames);
    });

    action *go_to_last_frame_action = new action("Go to Last Frame", "Alt+right");
    go_to_last_frame_action->connect_to(parent, [this]()
	{
		int _last_frame;
		if (in_out_visible)
			_last_frame = output;
		else
			_last_frame = last_frame;

		go_to_frame(_last_frame);
		frame_changed(_last_frame); // Signal
    });

    action *go_to_first_frame_action = new action("Go to First Frame", "Alt+left");
    go_to_first_frame_action->connect_to(parent, [this]()
	{
		int _first_frame;
		if (in_out_visible)
			_first_frame = input;
		else
			_first_frame = first_frame;

		go_to_frame(_first_frame);
		frame_changed(_first_frame); // Signal
    });
}

time_line::~time_line()
{
}

void time_line::set_frame_range(int _first_frame, int _last_frame)
{
	first_frame = _first_frame;
	last_frame = _last_frame;
	update();
}

void time_line::next_frame()
{
	int new_frame = frame + 1;
	go_to_frame(new_frame);
	frame_changed(new_frame); // Signal
}

void time_line::previous_frame()
{
	int new_frame = frame - 1;
	go_to_frame(new_frame);
	frame_changed(new_frame); // Signal
}

void time_line::next_frame_each(int frames)
{
	int new_frame = frame + frames;
	go_to_frame(new_frame);
	frame_changed(new_frame); // Signal
}

void time_line::previous_frame_each(int frames)
{
	int new_frame = frame - frames;
	go_to_frame(new_frame);
	frame_changed(new_frame); // Signal
}

void time_line::fit_switch()
{
	// conmutador entre el la ultima seleccion que se hizo,
	// y el ajuste al rango.
	if (is_fit_to_selector)
		fit_to_range(first_frame, last_frame);
	else
		fit_to_selector();
}

void time_line::fit_to_range(int _first_frame, int _last_frame)
{
	is_fit_to_selector = false;
    int padding = 5;

	// Equivalencia de padding tomando 100 frames de referencia
	padding = (_last_frame - _first_frame) * padding / 100;
	if (padding <= 1)
		padding = 1;
	//

    set_ortho(_first_frame - padding, _last_frame + padding, 0, 1);
}

void time_line::fit_to_selector()
{
	if (!selector.first && !selector.second)
		return;

	is_fit_to_selector = true;
    set_ortho(selector.first, selector.second, 0, 1);
}

void time_line::go_to_frame(int _frame)
{
	frame = _frame;
	ghost_frame = _frame;

	frame_edit->setText(QString::number(frame));
	update();
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
			go_to_frame(_frame);
			frame_changed(_frame); // Signal
		}
}

void time_line::set_in_out_visible(bool visible)
{
	in_out_visible = visible;
	update();
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
    left_button = event->button() == Qt::LeftButton;

	if (!right_button && in_out_visible)
	{
		dragging_input = click_over_in_out.first;
    	dragging_output = click_over_in_out.second;

    	change_in_out_with_control();
	}

    if (!qt::alt() && !qt::control() && !dragging_input && !dragging_output)
	{
		if (left_button)
		{
			go_to_frame(click_x_coords);
			frame_changed(click_x_coords); // Signal
		}
	}
	else
		panning(); // Signal

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

    if (!qt::alt() && !middle_button)
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
