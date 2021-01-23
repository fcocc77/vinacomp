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
	(*image) = QImage(1920, 1080, QImage::Format_RGB32);
	// (*image) = QImage(1920, 1080, QImage::Format_Mono);
	image->fill(Qt::black);

	float part = 255.0 / 1920;
	float value = 0;

	uchar *data = image->bits();

	int lines = image->bytesPerLine();
	QList <int> color = {0, 0, 0, 0};
	int c = 0;
	int channels = 4;

	int horizontal = 1920 * channels;
	for (int y = 0; y < 1080; y++)
	{
		// value += part;
		for (int x = 0; x < horizontal; x++)
		{
			data[(lines * y) + x] = color[c];

			if (c == 3)
				c = 0;
			else
				c++;

		}
	}

}
