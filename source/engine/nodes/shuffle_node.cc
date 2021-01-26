#include <shuffle_node.h>

shuffle_node::shuffle_node()
{

}

shuffle_node::~shuffle_node()
{

}

void shuffle_node::copy(QImage *src, QImage *dst, QList<int> channels_order)
{
	int new_red = channels_order[0];
	int new_green = channels_order[1];
	int new_blue = channels_order[2];
	int new_alpha = channels_order[3];

	bool red = new_red != 0;
	bool green = new_green != 1;
	bool blue = new_blue != 2;
	bool alpha = new_alpha != 3;

	int channels = 4;
	for (int y = 0; y < src->height(); y++)
	{
		uchar *src_line = src->scanLine(y);
		uchar *dst_line = dst->scanLine(y);

		if (blue)
		{
			for (int x = 0; x < src->width() * channels; x += channels)
			{
				if (new_blue == 0)
					dst_line[x] = src_line[x + 2]; // red to blue
				else if (new_blue == 1)
					dst_line[x] = src_line[x + 1]; // green to blue
				else if (new_blue == 3)
					dst_line[x] = src_line[x + 3]; // alpha to blue
				else if (new_blue == -2)
					dst_line[x] = 255; // white to blue
				else if (new_blue == -1)
					dst_line[x] = 0; // black to blue
			}
		}
		if (green)
		{
			for (int x = 1; x < src->width() * channels; x += channels)
			{
				if (new_green == 0)
					dst_line[x] = src_line[x + 1]; // red to green
				else if (new_green == 2)
					dst_line[x] = src_line[x - 1]; // blue to green
				else if (new_green == 3)
					dst_line[x] = src_line[x + 2]; // alpha to green
				else if (new_green == -2)
					dst_line[x] = 255; // white to green
				else if (new_green == -1)
					dst_line[x] = 0; // black to green
			}
		}
		if (red)
		{
			for (int x = 2; x < src->width() * channels; x += channels)
			{
				if (new_red == 1)
					dst_line[x] = src_line[x - 1]; // green to red
				else if (new_red == 2)
					dst_line[x] = src_line[x - 2]; // blue to red
				else if (new_red == 3)
					dst_line[x] = src_line[x + 1]; // alpha to red
				else if (new_red == -2)
					dst_line[x] = 255; // white to red
				else if (new_red == -1)
					dst_line[x] = 0; // black to red
			}
		}
		if (alpha)
		{
			for (int x = 3; x < src->width() * channels; x += channels)
			{
				if (new_alpha == 0)
					dst_line[x] = src_line[x - 1]; // red to alpha
				else if (new_alpha == 1)
					dst_line[x] = src_line[x - 2]; // green to alpha
				else if (new_alpha == 2)
					dst_line[x] = src_line[x - 3]; // blue to alpha
				else if (new_alpha == -2)
					dst_line[x] = 255; // white to alpha
				else if (new_alpha == -1)
					dst_line[x] = 0; // black to alpha
			}
		}
	}
}

void shuffle_node::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
	// antes de copiar crea un auxiliar para poder copiar los pixels entre la misma imagen
	QImage aux_image = *image;
	copy(image, &aux_image, {-2, 2, 2, 3});
	(*image) = aux_image;
}

