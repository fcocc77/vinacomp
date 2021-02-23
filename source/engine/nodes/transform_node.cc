#include <transform_node.h>

transform_node::transform_node()
{
	load_default_params("transform");
}

transform_node::~transform_node() { }

void transform_node::render(
	cv::Mat *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
}
