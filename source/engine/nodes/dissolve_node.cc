#include <dissolve_node.h>

dissolve_node::dissolve_node()
{
	load_default_params("dissolve");
}

dissolve_node::~dissolve_node() { }

void dissolve_node::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
}
