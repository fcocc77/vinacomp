#include <merge_node.h>

merge_node::merge_node()
{
	load_default_params("merge");
}

merge_node::~merge_node() { }

void merge_node::render( render_data *rdata, QJsonObject *params )
{
}
