#include <renderer.h>
#include <time_offset_node.h>

time_offset_node::time_offset_node()
{
    load_default_params("time_offset");
}

time_offset_node::~time_offset_node() {}

void time_offset_node::set_offset(QJsonObject *params, int &frame, QString node_name,
                                  QWidget *_renderer)
{
    int offset = get(params, "time_offset").toArray()[0].toInt();
    bool reverse = get(params, "reverse_input").toBool();

    frame -= offset;
    if (reverse)
    {
        auto frame_range = static_cast<renderer *>(_renderer)->get_frame_range(node_name);
        frame = -frame + frame_range.first + frame_range.second + 1;
    }
}

void time_offset_node::render(render_data *rdata, QJsonObject *params)
{
    int offset = get(params, "time_offset").toArray()[0].toInt();
    rdata->first_frame += offset;
    rdata->last_frame += offset;
}
