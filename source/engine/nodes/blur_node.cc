#include <blur_node.h>
#include <opencv2/opencv.hpp>

using namespace cv;

blur_node::blur_node()
{

}

blur_node::~blur_node()
{

}

void blur_node::render(
	QImage *image, 
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
	// Mat image_blurred_with_3x3_kernel;
	// GaussianBlur(data, image_blurred_with_3x3_kernel, Size(3, 3), 0);
}
