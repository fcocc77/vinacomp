#include <mirror_node.h>

mirror_node::mirror_node()
{
	load_default_params("mirror");
}

void flip_flop(cv::Mat *image, bool flip, bool flop)
{
	if (!flip && !flop)
		return;

	cv::Mat3f new_image(image->rows, image->cols);
    for( int y = 0; y < image->rows; y++ ) {
		int y_mirror = y;
		if (flip) 
			y_mirror = image->rows - y;

        for( int x = 0; x < image->cols; x++ ) {
			int x_mirror = x;
			if (flop)
				x_mirror = image->cols - x;

			new_image.at<cv::Vec3f>(y, x) = image->at<cv::Vec3f>(y_mirror, x_mirror);
		}
	}

	(*image) = new_image;
}

mirror_node::~mirror_node() { }

void mirror_node::render(
	cv::Mat *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
	bool flip = get(params, "flip").toBool();
	bool flop = get(params, "flop").toBool();

	flip_flop(image, flip, flop);
}
