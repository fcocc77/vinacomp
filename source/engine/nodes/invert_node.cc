#include <invert_node.h>

invert_node::invert_node(){}

invert_node::~invert_node(){}

void invert_node::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
	image->invertPixels();
}
