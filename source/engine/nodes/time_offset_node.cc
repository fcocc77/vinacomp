#include <time_offset_node.h>

time_offset_node::time_offset_node()
{

}

time_offset_node::~time_offset_node()
{

}

void time_offset_node::set_offset(QJsonObject *params, int &frame)
{
	int offset = params->value("time_offset").toArray()[0].toInt();
	bool reverse = params->value("reverse_input").toBool();
	frame -= offset;
}

void time_offset_node::render(
		QImage *image,
		QJsonObject *params, 
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox)
{
	int offset = params->value("time_offset").toArray()[0].toInt();
	frame_range.first += offset;
	frame_range.second += offset;
}
