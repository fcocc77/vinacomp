#include <viewer.h>

viewer::viewer()
	: current_frame(0)
	, frame_rate(24)
	, first_frame(1)
	, last_frame(100)
	, in_out(false)
	, input(0)
	, output(100)
{
    _viewer_gl = new viewer_gl();
    _viewer_gl->setObjectName("viewer_graphics");

    layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(1);
    this->setObjectName("viewer");

    setup_ui();
	player_init();
}

viewer::~viewer()
{
}

void viewer::set_frame(int frame)
{
	current_frame = frame;
	_time_line->go_to_frame(frame);
	// aqui se pone toda la actualizacion de la imagen del frame
	// ...
}

void viewer::set_frame_range(int _first_frame, int _last_frame)
{
	first_frame = _first_frame;
	last_frame = _last_frame;
	_time_line->set_frame_range(_first_frame, _last_frame);
}

void viewer::set_frame_rate(float rate)
{
	frame_rate = rate;
}

void viewer::enable_in_out(bool enable)
{
	in_out = enable;
	_time_line->set_in_out_visible(enable);
}

void viewer::set_in_out(int _input, int _output)
{
	input = _input;
	output = _output;

	enable_in_out(true);

	_time_line->update_in_out(input, output);
}
