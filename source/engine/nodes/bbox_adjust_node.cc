#include <bbox_adjust_node.h>

bbox_adjust_node::bbox_adjust_node()
{
	load_default_params("bbox_adjust");
}

bbox_adjust_node::~bbox_adjust_node() { }

void bbox_adjust_node::render(
	cv::Mat *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
}
