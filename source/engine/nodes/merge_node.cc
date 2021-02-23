#include <merge_node.h>

merge_node::merge_node()
{
	load_default_params("merge");
}

merge_node::~merge_node() { }

void merge_node::render(
	cv::Mat *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
}
