#include <frame_range_node.h>

frame_range_node::frame_range_node(){}

frame_range_node::~frame_range_node(){}

void frame_range_node::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{

	int first_frame = params->value("first_frame").toArray()[0].toInt();
	int last_frame = params->value("last_frame").toArray()[0].toInt();

	frame_range = {first_frame, last_frame};
}
