#include <time_offset_node.h>

time_offset_node::time_offset_node()
{
	load_default_params("time_offset");
}

time_offset_node::~time_offset_node()
{

}

void time_offset_node::set_offset(QJsonObject *params, int &frame)
{
	int offset = get("time_offset", params).toArray()[0].toInt();
	bool reverse = get("reverse_input", params).toBool();
	frame -= offset;
}

void time_offset_node::render(
		QImage *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox)
{
	int offset = get("time_offset", params).toArray()[0].toInt();
	frame_range.first += offset;
	frame_range.second += offset;
}
