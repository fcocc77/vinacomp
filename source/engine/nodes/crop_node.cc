#include <crop_node.h>

crop_node::crop_node()
{
	load_default_params("crop");
}

crop_node::~crop_node() { }

void crop_node::render( render_data *rdata, QJsonObject *params )
{
	QJsonArray box = get(params, "box").toArray();
	int x = box[0].toInt();
	int y = box[1].toInt();
	int w = box[2].toInt();
	int h = box[3].toInt();

	rdata->bbox.setRect(x, y, w, h);
	// cv::resize(rdata->image, rdata->image, cv::Size(500, 500), 0, 0, cv::INTER_CUBIC);

	int height = rdata->image.rows;
	int y_correct =  height - h;
	int h_correct = height - y_correct;
	y_correct -= y;

	// esto no copia los datos de imagen
	rdata->image = rdata->image(cv::Rect(x, y_correct, w, h_correct));
}
