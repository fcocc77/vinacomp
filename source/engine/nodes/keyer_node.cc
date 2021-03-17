#include <keyer_node.h>

keyer_node::keyer_node()
{
    load_default_params("keyer");
}

keyer_node::~keyer_node() {}

void keyer_node::render(render_data *rdata, QJsonObject *params) {}
