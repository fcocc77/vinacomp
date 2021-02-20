#include <grade_node.h>

grade_node::grade_node()
{
	load_default_params("grade");
}

grade_node::~grade_node() { }

void grade_node::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
}
