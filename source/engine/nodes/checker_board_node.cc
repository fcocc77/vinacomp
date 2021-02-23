#include <checker_board_node.h>

checker_board_node::checker_board_node()
{
	load_default_params("checker_board");
}

checker_board_node::~checker_board_node() { }

void checker_board_node::render(
	cv::Mat *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
	int size = get(params, "box_size").toInt();
	int border = get(params, "border").toInt();

	QColor color_a = get_color(params, "color_a");
	QColor color_b = get_color(params, "color_b");
	QColor border_color = get_color(params, "border_color");

	QColor background = Qt::black;

	int width = 1920;
	int height = 1080;

	// (*image) = QImage(width, height, QImage::Format_RGB32);
	// image->fill(background);

	// QPainter painter(image);
	// if (border)
		// painter.setPen(QPen(border_color, border));
	// else
		// painter.setPen(Qt::NoPen);


	// bool even_height = false;
	// int current_height = 0;

	// int height_count = float(height) / size;
	// int width_count = float(width) / size;

	// for (int h = 0; h <= height_count; h++)
	// {
		// bool even_width = even_height;
		// int current_width = 0;
		// for (int w = 0; w <= width_count; w++)
		// {
			// if (even_width)
				// painter.setBrush(QBrush(color_a));
			// else
				// painter.setBrush(QBrush(color_b));

			// painter.drawRect(current_width, current_height, size, size);

			// even_width = !even_width;
			// current_width += size;
		// }

		// even_height = !even_height;
		// current_height += size;
	// }
}
