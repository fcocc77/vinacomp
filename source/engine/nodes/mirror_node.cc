#include <mirror_node.h>

mirror_node::mirror_node()
{
	load_default_params("mirror");
}

mirror_node::~mirror_node() { }

void mirror_node::render(
	cv::Mat *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
	bool flip = get(params, "flip").toBool();
	bool flop = get(params, "flop").toBool();

	// if (flip || flop)
		// (*image) = image->mirrored(flip, flop);
}
