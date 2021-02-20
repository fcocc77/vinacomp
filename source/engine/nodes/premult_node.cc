#include <premult_node.h>

premult_node::premult_node()
{
	load_default_params("premult");
}

premult_node::~premult_node() { }

void premult_node::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
}
