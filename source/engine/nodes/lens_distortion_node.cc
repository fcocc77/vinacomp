#include <lens_distortion_node.h>

lens_distortion_node::lens_distortion_node()
{
	load_default_params("lens_distortion");
}

lens_distortion_node::~lens_distortion_node() { }

void lens_distortion_node::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
}
