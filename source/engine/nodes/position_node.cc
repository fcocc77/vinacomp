#include <position_node.h>

position_node::position_node(){
	load_default_params("position");
}

position_node::~position_node(){}

void position_node::render( render_data *rdata, QJsonObject *params )
{
	int x = get(params, "translate").toArray()[0].toInt();
	int y = get(params, "translate").toArray()[1].toInt();

	cv::Mat trans_mat = (cv::Mat_<double>(2,3) << 1, 0, x, 0, 1, y);
	cv::warpAffine(rdata->image, rdata->image, trans_mat, rdata->image.size());
}
