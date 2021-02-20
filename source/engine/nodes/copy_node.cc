#include <copy_node.h>

copy_node::copy_node()
{
	load_default_params("copy");
}

copy_node::~copy_node() { }

void copy_node::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
}
