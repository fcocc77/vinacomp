#include <reformat_node.h>

reformat_node::reformat_node()
{
	load_default_params("reformat");
}

reformat_node::~reformat_node(){}

void reformat_node::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
	if ( get("disable_node", params).toBool() ) return;

	int index = get("type", params).toArray()[0].toInt();
	int filter_index = get("filter", params).toArray()[0].toInt();

	// Filtro
	Qt::TransformationMode filter;
	if (filter_index == 0)
		filter = Qt::FastTransformation;
	else if (filter_index == 1)
		filter = Qt::SmoothTransformation;
	//

	int width = image->width();
	int height = image->height();
	int x, y;

	if (index == 0) // Format
	{
		x = get("format", params).toArray()[1].toArray()[0].toInt();
		y = get("format", params).toArray()[1].toArray()[1].toInt();
	}
	else if (index == 1) // Scale
	{
		float scale = get("scale", params).toDouble();

		x = width * scale;
		y = height * scale;
	}
	else if (index == 2) // Custom
	{
		x = get("custom", params).toArray()[0].toInt();
		y = get("custom", params).toArray()[1].toInt();
	}

	if (x != width || y != height)
		(*image) = image->scaled(x, y, Qt::IgnoreAspectRatio, filter);
}
