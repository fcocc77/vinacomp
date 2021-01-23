#include <radial_node.h>

radial_node::radial_node()
{
	load_default_params("radial");
}

radial_node::~radial_node() { }

void radial_node::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
	float softness = get(params, "softness").toDouble();
	QRect area = get_rect(params, "area");

	QColor color = get_color(params);
	QColor background = Qt::black;

	int width = 1920;
	int height = 1080;

	(*image) = QImage(width, height, QImage::Format_RGB32);
	image->fill(background);

	QPainter painter(image);

	if (softness == 0)
	{
		painter.setRenderHint(QPainter::Antialiasing);
		painter.setBrush(QBrush(color, Qt::SolidPattern));
		painter.drawEllipse(area);
	}
	else
	{
		// ! falta poder manipular el ancho y alto del radial
		QPointF centerPoint(500, 100);
		qreal centerRadius = 500;

		QRadialGradient radial(centerPoint, centerRadius);
		radial.setColorAt(0.0, color);
		radial.setColorAt(1.0 - softness, color);
		radial.setColorAt(1.0, background);

		QPen pen;
		pen.setWidth(1000);
		pen.setBrush(radial);

		painter.setPen(pen);
		painter.drawPoint(centerPoint);
	}
}
