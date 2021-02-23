#include <position_node.h>

position_node::position_node(){
	load_default_params("position");
}

position_node::~position_node(){}

void position_node::render(
	cv::Mat *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
	int x = get(params, "translate").toArray()[0].toInt();
	int y = get(params, "translate").toArray()[1].toInt();

	// cv::Mat img(image->height(), image->width(), CV_8UC4, image->bits(), image->bytesPerLine());

	cv::Mat trans_mat = (cv::Mat_<double>(2,3) << 1, 0, x, 0, 1, y);
	cv::warpAffine(*image, *image, trans_mat, image->size());
}
