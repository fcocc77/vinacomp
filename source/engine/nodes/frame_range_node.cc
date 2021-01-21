#include <frame_range_node.h>

frame_range_node::frame_range_node(){
	load_default_params("frame_range");
}

frame_range_node::~frame_range_node(){}

void frame_range_node::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
	int first_frame = get("first_frame", params).toArray()[0].toInt();
	int last_frame = get("last_frame", params).toArray()[0].toInt();

	frame_range = {first_frame, last_frame};
}
