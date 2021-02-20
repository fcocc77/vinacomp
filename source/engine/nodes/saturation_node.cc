#include <saturation_node.h>

saturation_node::saturation_node()
{
	load_default_params("saturation");
}

saturation_node::~saturation_node() { }

void saturation_node::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
}
