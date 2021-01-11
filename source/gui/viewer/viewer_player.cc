#include <viewer.h>

void viewer::player_init()
{
	int interval = 1;

	qtime_line = new QTimeLine(1000, this);
	qtime_line->setUpdateInterval(interval);
	qtime_line->setEasingCurve(QEasingCurve::Linear);

	connect(qtime_line, &QTimeLine::frameChanged, this, [=](int frame){
		set_frame(frame);
	});

	connect(qtime_line, &QTimeLine::finished, this, [=](){
		stop();
	});

	connect(_time_line, &time_line::frame_changed, this, [=](int frame){
		set_frame(frame);
	});
}

void viewer::set_frame(int frame)
{
	current_frame = frame;
	_time_line->go_to_frame(frame);
	// aqui se pone toda la actualizacion de la imagen del frame
	// ...
}

void viewer::play_forward()
{
	int first_frame = current_frame;
	int last_frame = 100;
	int total_frames = last_frame - first_frame + 1;
	int frame_rate = 24;
	int duration_ms = (total_frames * 1000) / frame_rate;

	qtime_line->setFrameRange(first_frame, last_frame);
	qtime_line->setDuration(duration_ms);

	play_forward_action->set_visible(false);
	stop_forward_action->set_visible(true);
	qtime_line->start();
}

void viewer::play_backward()
{
}

void viewer::stop()
{
	play_forward_action->set_visible(true);
	stop_forward_action->set_visible(false);
	qtime_line->stop();
}

void viewer::go_to_first_frame()
{
	set_frame(1);
	_time_line->go_to_frame(1);
}

void viewer::go_to_last_frame()
{
	set_frame(100);
	_time_line->go_to_frame(100);
}

void viewer::next_frame()
{
	_time_line->next_frame();
}

void viewer::previous_frame()
{
	_time_line->previous_frame();
}

void viewer::next_key_frame()
{
}

void viewer::previous_key_frame()
{
}

void viewer::skip_forward()
{
	int skip_frames = skip_frame_edit->text().toInt();
	_time_line->next_frame_each(skip_frames);
}

void viewer::skip_backward()
{
	int skip_frames = skip_frame_edit->text().toInt();
	_time_line->previous_frame_each(skip_frames);
}
