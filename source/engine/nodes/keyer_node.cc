#include <keyer_node.h>

keyer_node::keyer_node()
{
	load_default_params("keyer");
}

keyer_node::~keyer_node() { }

void keyer_node::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
}
