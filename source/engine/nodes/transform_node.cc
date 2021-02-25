#include <transform_node.h>

transform_node::transform_node()
{
	load_default_params("transform");
}

transform_node::~transform_node() { }

void transform_node::render( render_data *rdata, QJsonObject *params )
{
}
