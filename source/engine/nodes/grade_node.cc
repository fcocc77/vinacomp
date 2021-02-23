#include <grade_node.h>

grade_node::grade_node()
{
	load_default_params("grade");
}

grade_node::~grade_node() { }

void grade_node::render(
	cv::Mat *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
	QJsonArray gain = get(params, "gain").toArray();
	QJsonArray gamma = get(params, "gamma").toArray();

	float gamma_red = (1 - gamma[0].toDouble()) + 1;
	float gamma_green = (1 - gamma[1].toDouble()) + 1;
	float gamma_blue = (1 - gamma[2].toDouble()) + 1;

	float gain_red = gain[0].toDouble();
	float gain_green = gain[1].toDouble();
	float gain_blue = gain[2].toDouble();

    for( int y = 0; y < image->rows; y++ ) {
        for( int x = 0; x < image->cols; x++ ) {
			cv::Vec3f &pixel = image->at<cv::Vec3f>(y, x);

			float &red = pixel[2];
			float &green = pixel[1];
			float &blue = pixel[0];

			red *= gain_red;
			green *= gain_green;
			blue *= gain_blue;

			red = 255 * pow(red / 255.0, gamma_red);
			green = 255 * pow(green / 255.0, gamma_green);
			blue = 255 * pow(blue / 255.0, gamma_blue);
        }
    }
}
