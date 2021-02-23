#include <frame_hold_node.h>

frame_hold_node::frame_hold_node()
{
	load_default_params("frame_hold");
}

frame_hold_node::~frame_hold_node() { }

void frame_hold_node::render(
	cv::Mat *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
}
