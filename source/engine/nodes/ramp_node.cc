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
	QJsonArray point0 = get(params, "point0").toArray();
	QJsonArray point1 = get(params, "point1").toArray();
	QPoint p0 = {point0[0].toInt(), point0[1].toInt()};
	QPoint p1 = {point1[0].toInt(), point1[1].toInt()};

	int x = get(params, "format").toArray()[1].toArray()[0].toInt();
	int y = get(params, "format").toArray()[1].toArray()[1].toInt();

	QColor color = get_color(params);

	QLinearGradient ramp(p0, p1);
    ramp.setColorAt(0, Qt::black);
    ramp.setColorAt(1, color);

	(*image) = QImage(x, y, QImage::Format_RGB32);
	QPainter painter(image);
	painter.fillRect(image->rect(), ramp);
}
