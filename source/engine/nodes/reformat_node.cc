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
	int index = get("type", params).toInt();
	int filter_index = get("filter", params).toInt();

	// Filtro
	Qt::TransformationMode filter;
	if (filter_index == 0)
		filter = Qt::FastTransformation;
	else if (filter_index == 1)
		filter = Qt::SmoothTransformation;
	//

	if (index == 1) // Scale
	{
		float scale = get("scale", params).toDouble();

		int width = image->width();
		int height = image->height();

		int x = width * scale;
		int y = height * scale;

		if (x != width || y != height)
			(*image) = image->scaled(x, y, Qt::IgnoreAspectRatio, filter);
	}
	else if (index == 2) // Custom
	{
		int x = get("custom", params).toArray()[0].toInt();
		int y = get("custom", params).toArray()[1].toInt();

		(*image) = image->scaled(x, y, Qt::IgnoreAspectRatio, filter);
	}
}
