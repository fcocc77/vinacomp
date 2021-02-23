#include <erode_node.h>

erode_node::erode_node()
{
	load_default_params("erode");
}

erode_node::~erode_node() { }

void erode_node::render(
	cv::Mat *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
}
