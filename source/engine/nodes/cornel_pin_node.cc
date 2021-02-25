#include <cornel_pin_node.h>

cornel_pin_node::cornel_pin_node(){
	load_default_params("cornel_pin");
}

cornel_pin_node::~cornel_pin_node(){}

void cornel_pin_node::render( render_data *rdata, QJsonObject *params )
{
	int x = get(params, "translate").toArray()[0].toInt();
	int y = get(params, "translate").toArray()[1].toInt();

	// cv::Mat img(image->height(), image->width(), CV_8UC4, image->bits(), image->bytesPerLine());

}
