#include <sharpen_node.h>

sharpen_node::sharpen_node()
{
	load_default_params("sharpen");
}

sharpen_node::~sharpen_node() { }

void sharpen_node::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
	float amount = get(params, "amount").toDouble();
	float size = get(params, "size").toDouble();

	cv::Mat img(image->height(), image->width(), CV_8UC4, image->bits(), image->bytesPerLine());
	// cv::blur(img, img, cv::Size(size, size));

	// cv::GaussianBlur(frame, image, cv::Size(0, 0), 3);
	// cv::addWeighted(0, 1.5, img, -0.5, 0, img);

	// cv::Mat sharpenedLena;
	cv::Mat kernel = (cv::Mat_<float>(3, 3) << 0, -1, 0, -1, 4, -1, 0, -1, 0);
	cv::filter2D(img, img, img.depth(), kernel);
}
