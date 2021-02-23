#include <frame_range_node.h>

frame_range_node::frame_range_node(){
	load_default_params("frame_range");
}

frame_range_node::~frame_range_node(){}

pair <int, int> frame_range_node::get_frame_range(QJsonObject *params) const
{
	int first_frame = get(params, "frame_range").toArray()[0].toInt();
	int last_frame = get(params, "frame_range").toArray()[1].toInt();

	return {first_frame, last_frame};
}

void frame_range_node::render(
	cv::Mat *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
	frame_range = get_frame_range(params);
}
