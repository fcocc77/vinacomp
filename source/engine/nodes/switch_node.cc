#include <switch_node.h>

switch_node::switch_node()
{
	load_default_params("switch");
}

switch_node::~switch_node() { }

void switch_node::render(
	cv::Mat *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
}
