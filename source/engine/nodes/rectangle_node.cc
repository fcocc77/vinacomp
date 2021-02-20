#include <rectangle_node.h>

rectangle_node::rectangle_node()
{
	load_default_params("rectangle");
}

rectangle_node::~rectangle_node() { }

void rectangle_node::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
}
