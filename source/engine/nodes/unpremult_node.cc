#include <unpremult_node.h>

unpremult_node::unpremult_node()
{
    load_default_params("unpremult");
}

unpremult_node::~unpremult_node() {}

void unpremult_node::render(render_data *rdata, QJsonObject *params) {}
