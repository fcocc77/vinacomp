#include <frame_hold_node.h>

frame_hold_node::frame_hold_node()
{
    load_default_params("frame_hold");
}

frame_hold_node::~frame_hold_node() {}

void frame_hold_node::render(render_data *rdata, QJsonObject *params) {}
