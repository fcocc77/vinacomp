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
	QJsonArray blackpoint = get(params, "blackpoint").toArray();
	QJsonArray whitepoint = get(params, "whitepoint").toArray();
	QJsonArray gain = get(params, "gain").toArray();
	QJsonArray multiply = get(params, "multiply").toArray();
	QJsonArray gamma = get(params, "gamma").toArray();

	float black_red = blackpoint[0].toDouble();
	float black_green = blackpoint[1].toDouble();
	float black_blue = blackpoint[2].toDouble();

	float white_red = whitepoint[0].toDouble();
	float white_green = whitepoint[1].toDouble();
	float white_blue = whitepoint[2].toDouble();

	float gain_red = gain[0].toDouble() * multiply[0].toDouble();
	float gain_green = gain[1].toDouble() * multiply[1].toDouble();
	float gain_blue = gain[2].toDouble() * multiply[2].toDouble();

	float gamma_red = (1 - gamma[0].toDouble()) + 1;
	float gamma_green = (1 - gamma[1].toDouble()) + 1;
	float gamma_blue = (1 - gamma[2].toDouble()) + 1;

	// levels
	float low = black_red * 255;
	float high = white_red * 255;
	float max = 255;
	float min = 0;
	float levels_alpha = ((max - min) / (high - low));
	//

    for( int y = 0; y < image->rows; y++ ) {
        for( int x = 0; x < image->cols; x++ ) {
			cv::Vec3f &pixel = image->at<cv::Vec3f>(y, x);

			float &red = pixel[2];
			float &green = pixel[1];
			float &blue = pixel[0];

			// levels
			red = min + (red - low) * levels_alpha;
			green = min + (green - low) * levels_alpha;
			blue = min + (blue - low) * levels_alpha;
			//

			// gain
			red *= gain_red;
			green *= gain_green;
			blue *= gain_blue;
			//

			// gamma
			if (gamma_red != 1)
				red = pow(red / 255.0, gamma_red) * 255.0;
			if (gamma_green != 1)
				green = pow(green / 255.0, gamma_green) * 255.0;
			if (gamma_blue != 1)
				blue = pow(blue / 255.0, gamma_blue) * 255.0;
			//
        }
    }
}
