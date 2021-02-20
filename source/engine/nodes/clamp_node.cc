#include <clamp_node.h>

clamp_node::clamp_node()
{
	load_default_params("clamp");
}

clamp_node::~clamp_node() { }

void clamp_node::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
}
