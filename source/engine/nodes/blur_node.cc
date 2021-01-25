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

	cv::Mat img(image->height(), image->width(), CV_8UC4, image->bits(), image->bytesPerLine());
	if (blur_size > 0)
		// cv::GaussianBlur(img, img, cv::Size(99, 99), 0);
		// cv::boxFilter(img, img, -1, cv::Size(blur_size, blur_size));
		cv::blur(img, img, cv::Size(blur_size, blur_size));
}
