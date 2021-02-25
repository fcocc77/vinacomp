#include <copy_node.h>

copy_node::copy_node()
{
	load_default_params("copy");
}

copy_node::~copy_node() { }

void copy_node::render( render_data *rdata, QJsonObject *params )
{
}
