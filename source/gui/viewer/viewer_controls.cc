#include <viewer.h>

void viewer::change_layer(QString layer)
{
    current_layer = layer;
    update_render(true);
}

void viewer::change_channel(int index)
{
    if (visible_channel == (index - 1))
    {
        visible_channel = -1;
        display_channel->set_index(0, false);
    }
    else
    {
        visible_channel = index - 1;
        display_channel->set_index(index, false);
    }

    _viewer_gl->isolate_channel(visible_channel);
    update_render();
}

void viewer::purge_ram(){}

void viewer::set_handler_snap(bool enable){}

void viewer::set_outside_pixels(bool enable){}

void viewer::set_render_area(bool enable){}

void viewer::set_proxy(bool enable){}

void viewer::set_multi_lines(bool enable){}
