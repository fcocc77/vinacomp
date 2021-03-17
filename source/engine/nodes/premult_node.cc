#include <premult_node.h>

premult_node::premult_node()
{
    load_default_params("premult");
}

premult_node::~premult_node() {}

void premult_node::render(render_data *rdata, QJsonObject *params) {}
