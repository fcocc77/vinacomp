#include <viewer.h>
#include <vinacomp.h>
#include <QTime>
#include <qt.h>

void viewer::player_init()
{
    frame_rate_menu->set_value(QString::number(frame_rate));
    play_back_options->set_value("repeat");
}

void viewer::set_playing_option(QString option)
{
    playing_option = option;
}

void viewer::play(int direction)
{
    stop();

    playing = true;
    play_direction = direction;
    frame_rate_sum = 0;
    frame_rate_count = 0;

    set_frame(project->frame);

    static_cast<vinacomp *>(_vinacomp)
        ->get_curve_editor()
        ->get_curve_view()
        ->set_cursor_visibility(false);

    play_forward_action->set_visible(false);
    stop_forward_action->set_visible(true);
    play_backward_action->set_visible(false);
    stop_backward_action->set_visible(true);
}

void viewer::stop()
{
    playing = false;

    static_cast<vinacomp *>(_vinacomp)
        ->get_curve_editor()
        ->get_curve_view()
        ->set_cursor_visibility(true);

    play_forward_action->set_visible(true);
    play_backward_action->set_visible(true);
    stop_forward_action->set_visible(false);
    stop_backward_action->set_visible(false);

    frame_rate_menu->change_label(QString::number((int)frame_rate) + " fps");
    qt::set_property(frame_rate_menu, "rate", "normal");
}

void viewer::calculate_frame_rate()
{
    int average_count = 5;

    int current_time = QTime::currentTime().msecsSinceStartOfDay();

    float elapsed = current_time - last_time;
    float rate = round(1000.0 / elapsed);

    last_time = current_time;

    frame_rate_sum += rate;
    frame_rate_count ++;

    if (frame_rate_count <= average_count)
        return;

    float final_rate = frame_rate_sum / (float)frame_rate_count;
    float diff = frame_rate - final_rate;

    // cambia el color si hay una diferencia de frame rate de 5 fps
    if (diff >= 5)
        qt::set_property(frame_rate_menu, "rate", "low");
    else
        qt::set_property(frame_rate_menu, "rate", "good");

    frame_rate_menu->change_label(QString::number(round(final_rate)) + " fps");

    frame_rate_sum = 0;
    frame_rate_count = 0;
}

void viewer::playing_now()
{
    // acciones a seguir si es que esta en play

    if (!playing)
        return;

    calculate_frame_rate();

    int frame = project->frame;

    auto frame_range = get_current_range();
    int _first_frame = frame_range.first;
    int _last_frame = frame_range.second;

    if (playing_option == "bounce")
    {
        if (frame >= _last_frame)
            play_direction = vina::left;

        if (frame <= _first_frame)
            play_direction = vina::right;
    }
    else if (playing_option == "repeat")
    {
        if (frame >= _last_frame)
            frame = _first_frame;
        else if (frame <= _first_frame)
            frame = _last_frame;
    }
    else if (playing_option == "stop")
    {
        if (frame >= _last_frame && play_direction == vina::right)
        {
            stop();
            return;
        }
        else if (frame <= _first_frame && play_direction == vina::left)
        {
            stop();
            return;
        }
    }

    if (play_direction == vina::right)
        frame++;
    else
        frame--;

    set_frame(frame);
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
    stop();

    int start_frame = get_current_range().first;
    set_frame(start_frame);
    _time_line->go_to_frame(start_frame);
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

void viewer::next_key_frame() {}

void viewer::previous_key_frame() {}

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
