#include <constant_node.h>

constant_node::constant_node()
{
	load_default_params("constant");
}

constant_node::~constant_node() { }

void constant_node::render(
	cv::Mat *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
	int first_frame = get(params, "frame_range").toArray()[0].toInt();
	int last_frame = get(params, "frame_range").toArray()[1].toInt();

	int x = get(params, "format").toArray()[1].toArray()[0].toInt();
	int y = get(params, "format").toArray()[1].toArray()[1].toInt();

	QColor color = get_color(params);

	cv::Mat3f solid(1080, 1920);
	solid.setTo(cv::Scalar(color.red(), color.green(), color.blue()));

	(*image) = solid;

	frame_range = {first_frame, last_frame};
}
