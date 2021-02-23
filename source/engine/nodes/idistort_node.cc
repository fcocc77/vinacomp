#include <idistort_node.h>

idistort_node::idistort_node()
{
	load_default_params("idistort");
}

idistort_node::~idistort_node() { }

void idistort_node::render(
	cv::Mat *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
}
