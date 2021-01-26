#include <blur_node.h>


blur_node::blur_node()
{
	load_default_params("blur");
}

blur_node::~blur_node() { }

void blur_node::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
	int blur_size = get(params, "size").toDouble();
	int filter_index = get(params, "filter").toArray()[0].toInt();

	cv::Mat img(image->height(), image->width(), CV_8UC4, image->bits(), image->bytesPerLine());
	if (blur_size > 0)
	{
		if (filter_index == 0)
			cv::blur(img, img, cv::Size(blur_size, blur_size));
		else if (filter_index == 3)
			cv::GaussianBlur(img, img, cv::Size(99, 99), 0);
	}
}
