#include <bbox_adjust_node.h>

bbox_adjust_node::bbox_adjust_node()
{
    load_default_params("bbox_adjust");
}

bbox_adjust_node::~bbox_adjust_node() {}

void bbox_adjust_node::render(render_data *rdata, QJsonObject *params) {}
