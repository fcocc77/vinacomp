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
	frame -= offset;
}
