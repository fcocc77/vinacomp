#include <ramp_node.h>

ramp_node::ramp_node()
{
	load_default_params("ramp");
}

ramp_node::~ramp_node() { }

void ramp_node::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
	QJsonArray point0 = get("point0", params).toArray();
	QJsonArray point1 = get("point1", params).toArray();
	QPoint p0 = {point0[0].toInt(), point0[1].toInt()};
	QPoint p1 = {point1[0].toInt(), point1[1].toInt()};

	int x = get("format", params).toArray()[1].toArray()[0].toInt();
	int y = get("format", params).toArray()[1].toArray()[1].toInt();

	QJsonArray colors = get("color", params).toArray();
	float red = colors[0].toDouble() * 255;
	float green = colors[1].toDouble() * 255;
	float blue = colors[2].toDouble() * 255;
	float alpha = colors[3].toDouble() * 255;

	QLinearGradient ramp(p0, p1);
    ramp.setColorAt(0, Qt::black);
    ramp.setColorAt(1, QColor(red, green, blue, alpha));

	(*image) = QImage(x, y, QImage::Format_RGB32);
	QPainter painter(image);
	painter.fillRect(image->rect(), ramp);
}
