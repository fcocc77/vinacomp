#include <crop_node.h>

crop_node::crop_node()
{
	load_default_params("crop");
}

crop_node::~crop_node() { }

void crop_node::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
}
