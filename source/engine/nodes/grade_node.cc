#include <grade_node.h>

grade_node::grade_node()
{
	load_default_params("grade");
}

grade_node::~grade_node() { }

QImage grade_node::mat_to_qimage(const cv::Mat3b &img)
{
	// image->convertToFormat(QImage::Format_RGB32);
	QImage qimage(img.cols, img.rows, QImage::Format_RGB32);
	qimage.fill(Qt::green);

	for (int y = 0; y < img.rows; y++)
	{
		const cv::Vec3b *srcrow = img[y];
		// QRgb *destrow = (QRgb*)dest.scanLine(y);

		// uchar *line = qimage->scanLine(y);
		for (int x = 0; x < img.cols; x++)
		{
			int red = srcrow[x][2];
			int green = srcrow[x][1];
			int blue = srcrow[x][0];
			print(red);
			qimage.setPixelColor(x, y, QColor(red, green, blue));

			// unsigned int color = srcrow[x] * 255;
			// const cv::Vec3b *srcrow = src[y];

			// line[x]
		}
	}
	return qimage;
}

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

// CV_32F

	// IMREAD_COLOR - 3 channel BGR
	cv::Mat img = cv::imread("/home/pancho/Desktop/3pixel.jpg", cv::IMREAD_COLOR);
	img.convertTo(img, CV_8U);

	// (*image) = mat_to_qimage(img);



	// int channels = 4;
	// for (int y = 0; y < image->height(); y++)
	// {
		// uchar *line = image->scanLine(y);

		// for (int x = 0; x < image->width() * channels; x += channels)
		// {
			// // float b = line[x] / 255.0;
			// // line[x] = 255 * pow(b, gamma_blue);

			// line[x] *= gain_blue;
		// }

		// for (int x = 1; x < image->width() * channels; x += channels)
		// {
			// // float g = line[x] / 255.0;
			// // line[x] = 255 * pow(g, gamma_green);
		// }

		// for (int x = 2; x < image->width() * channels; x += channels)
		// {
			// // float r = line[x] / 255.0;
			// // line[x] = 255 * pow(r, gamma_red);
		// }
	// }

}
