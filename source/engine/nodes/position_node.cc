#include <position_node.h>

position_node::position_node(){
	load_default_params("position");
}

position_node::~position_node(){}

void position_node::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
	int x = get("translate", params).toArray()[0].toInt();
	int y = get("translate", params).toArray()[1].toInt();

	for ( int x = 0; x < 1920; x++)
		for ( int y = 0; y < 1080; y++)
			QColor color = image->pixel(100, 108);

	image->setOffset({x, y});
}
