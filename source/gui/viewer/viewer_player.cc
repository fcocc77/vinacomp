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

	set_frame_rate(frame_rate);
}

void viewer::set_frame(int frame)
{
	current_frame = frame;
	_time_line->go_to_frame(frame);
	// aqui se pone toda la actualizacion de la imagen del frame
	// ...
}
void viewer::set_frame_rate(float rate)
{
	frame_rate = rate;
	frame_rate_edit->setText(QString::number(frame_rate));
}

void viewer::play(QTimeLine::Direction direction)
{
	stop();

	int end_frame, total_frames;
	int start_frame = current_frame;

	if (direction == QTimeLine::Forward)
	{
		end_frame = 100;
		total_frames = end_frame - start_frame + 1;
		qtime_line->setFrameRange(start_frame, end_frame);

		play_forward_action->set_visible(false);
		stop_forward_action->set_visible(true);
	}
	else
	{
		end_frame = 1;
		total_frames = start_frame - end_frame + 1;
		qtime_line->setFrameRange(end_frame, start_frame);

		play_backward_action->set_visible(false);
		stop_backward_action->set_visible(true);
	}

	int duration_ms = (total_frames * 1000) / frame_rate;

	qtime_line->setDuration(duration_ms);
	qtime_line->setDirection(direction);

	qtime_line->start();
}

void viewer::stop()
{
	play_forward_action->set_visible(true);
	play_backward_action->set_visible(true);
	stop_forward_action->set_visible(false);
	stop_backward_action->set_visible(false);
	qtime_line->stop();
}

void viewer::go_to_first_frame()
{
	bool playing = qtime_line->state();
	stop();

	set_frame(1);
	_time_line->go_to_frame(1);

	if (playing)
		play(qtime_line->direction());
}

void viewer::go_to_last_frame()
{
	stop();
	set_frame(100);
	_time_line->go_to_frame(100);
}

void viewer::next_frame()
{
	stop();
	_time_line->next_frame();
}

void viewer::previous_frame()
{
	stop();
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
	stop();
	int skip_frames = skip_frame_edit->text().toInt();
	_time_line->next_frame_each(skip_frames);
}

void viewer::skip_backward()
{
	stop();
	int skip_frames = skip_frame_edit->text().toInt();
	_time_line->previous_frame_each(skip_frames);
}
