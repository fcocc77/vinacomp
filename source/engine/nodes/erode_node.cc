#include <erode_node.h>

erode_node::erode_node()
{
    load_default_params("erode");
}

erode_node::~erode_node() {}

void erode_node::render(render_data *rdata, QJsonObject *params) {}
