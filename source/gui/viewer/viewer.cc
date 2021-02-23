#include <viewer.h>
#include <vinacomp.h>
#include <project_settings.h>

viewer::viewer(QString _name, project_struct *_project, renderer *__renderer, QWidget *__vinacomp)
	: name(_name)
	, project(_project)
	, _renderer(__renderer)
	, _vinacomp(__vinacomp)

	, current_frame(0)
	, frame_rate(60)
	, first_frame(1)
	, last_frame(100)
	, in_out(false)
	, input(0)
	, output(100)
	, global_range(true)
	, render_pause(false)
	, visible_channel(-1)
{
    _viewer_gl = new viewer_gl();
    _viewer_gl->setObjectName("viewer_graphics");

	image = new cv::Mat;

    layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(1);
    this->setObjectName("viewer");

    setup_ui();
	player_init();

	connect(qtime_line, &QTimeLine::frameChanged, this, [=](int frame){
		set_frame(frame);
	});

	connect(qtime_line, &QTimeLine::finished, this, [=](){
		play_finished();
	});

	connect(_time_line, &time_line::frame_changed, this, [=](int frame){
		set_frame(frame);
	});

	connect(_time_line, &time_line::in_out_changed, this, [=](int _input, int _output){
		set_in_out(_input, _output);
	});

	connect(_time_line, &time_line::panning, this, [=](){
		if (global_range)
			range_way_menu->set_index(0, false);
		else
			range_way_menu->set_index(1, false);
	});

	connect(_viewer_gl, &viewer_gl::right_click, this, [=](){
		menu->move(QCursor::pos());
		menu->show();
	});
}

viewer::~viewer()
{
}

QString viewer::get_name() const
{
	return name;
}

void viewer::set_frame(int frame)
{
	current_frame = frame;
	_time_line->go_to_frame(frame);

	update_render();
}

void viewer::update_render()
{
	if (render_pause)
		return;

	pair<int, int> frame_range;
	QRect bbox;
	_renderer->render(image, current_frame, name, frame_range, bbox);

	// isolate_channel(image, visible_channel);

	_viewer_gl->set_image(image);
	if (input_range_way == "input")
		set_frame_range(frame_range.first, frame_range.second);
}

void viewer::update_input_range()
{
	if (input_range_way == "global")
	{
		auto frame_range = dynamic_cast<vinacomp*>(_vinacomp)->get_project_settings()->get_frame_range();
		set_frame_range(frame_range.first, frame_range.second);
		global_range = true;
	}
	else if (input_range_way == "input")
	{
		update_render();
		global_range = false;
	}
	else if (input_range_way == "inout")
	{
		if (in_out)
			_time_line->fit_to_range(input, output);

		return;
	}

	_time_line->fit_to_range(first_frame, last_frame);
}

void viewer::set_frame_range(int _first_frame, int _last_frame)
{
	if (_first_frame == first_frame && _last_frame == last_frame)
		return;

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
	visibility_in_out_action->set_checked(enable);
	input_action->set_checked(enable);
	output_action->set_checked(enable);
	_time_line->set_in_out_visible(enable);
}

void viewer::set_in_out(int _input, int _output)
{
	input = _input;
	output = _output;

	enable_in_out(true);

	_time_line->update_in_out(input, output);
}
