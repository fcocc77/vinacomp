#include <noise_node.h>

noise_node::noise_node()
{
	load_default_params("noise");
}

noise_node::~noise_node() { }

void noise_node::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
	uchar *data = image->bits();
	int lines = image->bytesPerLine();
	int channels = 4;

	float part = 255.0 / image->height();
	float value = 0;

	for (int y = 0; y < image->height(); y++)
	{
		value += part;
		for (int x = 0; x < image->width(); x++)
		{
			int blue_index = (y * lines) + (x * channels + 0);
			int green_index = (y * lines) + (x * channels + 1);
			int red_index = (y * lines) + (x * channels + 2);

			data[blue_index] = value;
			data[green_index] = value;
			data[red_index] = value;
		}
	}
}
