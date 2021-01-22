#include <viewer.h>

void viewer::player_init()
{
	int interval = 1;

	qtime_line = new QTimeLine(1000, this);
	qtime_line->setUpdateInterval(interval);
	qtime_line->setEasingCurve(QEasingCurve::Linear);

	frame_rate_menu->set_value(QString::number(frame_rate));
	play_back_options->set_value("repeat");
}

void viewer::set_playing_option(QString option)
{
	playing_option = option;
}

void viewer::play_finished()
{
	if (playing_option == "bounce")
	{
		if (qtime_line->direction() == QTimeLine::Forward)
			play(QTimeLine::Backward);
		else
			play(QTimeLine::Forward);
	}
	else if (playing_option == "repeat")
	{
		if (qtime_line->direction() == QTimeLine::Forward)
		{
			current_frame = get_current_range().first;
			play(QTimeLine::Forward);
		}
		else
		{
			current_frame = get_current_range().second;
			play(QTimeLine::Backward);
		}
	}
	else
		stop();
}

void viewer::play(QTimeLine::Direction direction)
{
	stop();

	int end_frame, total_frames, start_frame;
	auto frame_range = get_current_range();

	// si el cursor esta fuera del rango, inicia con el primer cuadro del rango actual
	if (current_frame > frame_range.second || current_frame < frame_range.first)
		start_frame = frame_range.first;
	else
		start_frame = current_frame;
	//

	if (direction == QTimeLine::Forward)
	{
		end_frame = frame_range.second;
		total_frames = end_frame - start_frame + 1;
		qtime_line->setFrameRange(start_frame, end_frame);
	}
	else
	{
		end_frame = frame_range.first;
		total_frames = start_frame - end_frame + 1;
		qtime_line->setFrameRange(end_frame, start_frame);
	}

	play_forward_action->set_visible(false);
	stop_forward_action->set_visible(true);
	play_backward_action->set_visible(false);
	stop_backward_action->set_visible(true);

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

pair<int, int> viewer::get_current_range() const
{
	// obtiene el rango actual, ya que puede ser con el input y output activado
	// o puese ser el rango general o el de entrada.

	if (in_out)
		return {input, output};
	else
		return {first_frame, last_frame};
}

void viewer::go_to_first_frame()
{
	bool playing = qtime_line->state();
	stop();

	int start_frame = get_current_range().first;
	set_frame(start_frame);
	_time_line->go_to_frame(start_frame);

	if (playing)
		play(qtime_line->direction());
}

void viewer::go_to_last_frame()
{
	stop();
	int end_frame = get_current_range().second;
	set_frame(end_frame);
	_time_line->go_to_frame(end_frame);
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
