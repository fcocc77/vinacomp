#ifndef FRAME_RANGE_NODE_H
#define FRAME_RANGE_NODE_H

#include <node_engine.h>

class frame_range_node : public node_engine
{
public:
	frame_range_node();
	~frame_range_node();

	void render(
		QImage *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox) override;
};

#endif // FRAME_RANGE_NODE_H
