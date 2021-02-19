#include <grid_node.h>

grid_node::grid_node()
{
	load_default_params("grid");
}

grid_node::~grid_node() { }

void grid_node::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
	float number = get(params, "number").toDouble();
	float size = get(params, "size").toDouble();
	bool dashdot = get(params, "dashdot").toBool();

	QColor color = get_color(params);
	QColor background = Qt::black;

	int width = 1920;
	int height = 1080;

	(*image) = QImage(width, height, QImage::Format_RGB32);
	image->fill(background);

	QPainter painter(image);

	if (dashdot)
	{
		painter.setPen(QPen(color, size, Qt::DashDotLine, Qt::RoundCap));
		painter.setRenderHint(QPainter::Antialiasing);
	}
	else
		painter.setPen(QPen(color, size));


	float height_number = number;
	float width_number = number;

	float height_part = float( height ) / height_number;
	float height_current = 0;
	for (int h = 0; h <= height_number; h++)
	{
		painter.drawLine(0, height_current, width, height_current);
		height_current += height_part;
	}

	float width_part = float( width ) / width_number;
	float width_current = 0;
	for (int w = 0; w <= width_number; w++)
	{
		painter.drawLine(width_current, 0, width_current, height);
		width_current += width_part;
	}
}
