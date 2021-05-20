#include <roto_node.h>

roto_node::roto_node()
{
    load_default_params("roto");
}

roto_node::~roto_node() {}

void roto_node::render(render_data *rdata, QJsonObject *params) {}
