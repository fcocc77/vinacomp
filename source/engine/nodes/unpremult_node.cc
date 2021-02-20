#include <unpremult_node.h>

unpremult_node::unpremult_node()
{
	load_default_params("unpremult");
}

unpremult_node::~unpremult_node() { }

void unpremult_node::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
}
