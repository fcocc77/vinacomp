#include <viewer.h>

void viewer::play_forward()
{
}

void viewer::play_backward()
{
}

void viewer::go_to_first_frame()
{
	_time_line->go_to_frame(1);
}

void viewer::go_to_last_frame()
{
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
	_time_line->previous_frame_each(10);
}
