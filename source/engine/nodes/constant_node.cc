#include <constant_node.h>

constant_node::constant_node()
{
	load_default_params("constant");
}

constant_node::~constant_node() { }

void constant_node::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
	int first_frame = get("frame_range", params).toArray()[0].toInt();
	int last_frame = get("frame_range", params).toArray()[1].toInt();

	int x = get("format", params).toArray()[1].toArray()[0].toInt();
	int y = get("format", params).toArray()[1].toArray()[1].toInt();

	QJsonArray colors = get("color", params).toArray();
	float red = colors[0].toDouble() * 255;
	float green = colors[1].toDouble() * 255;
	float blue = colors[2].toDouble() * 255;
	float alpha = colors[3].toDouble() * 255;

	(*image) = QImage(x, y, QImage::Format_RGB32);
	image->fill(QColor(red, green, blue, alpha));

	frame_range = {first_frame, last_frame};
}
