#include <switch_node.h>

switch_node::switch_node()
{
	load_default_params("switch");
}

switch_node::~switch_node() { }

void switch_node::render( render_data *rdata, QJsonObject *params )
{
}
