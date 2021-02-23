#include <edge_detect_node.h>

edge_detect_node::edge_detect_node()
{
	load_default_params("edge_detect");
}

edge_detect_node::~edge_detect_node() { }

void edge_detect_node::render(
	cv::Mat *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
}
