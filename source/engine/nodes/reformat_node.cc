#include <reformat_node.h>

reformat_node::reformat_node(){}

reformat_node::~reformat_node(){}

void reformat_node::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
	(*image) = image->scaledToWidth(100);
}
