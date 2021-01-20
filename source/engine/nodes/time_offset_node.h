#ifndef TIME_OFFSET_NODE_H
#define TIME_OFFSET_NODE_H

#include <node_engine.h>

class time_offset_node : public node_engine
{
public:
	time_offset_node();
	~time_offset_node();

	void set_offset(QJsonObject *params, int &frame);
};

#endif // TIME_OFFSET_NODE_H
