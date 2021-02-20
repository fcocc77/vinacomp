#ifndef FRAME_HOLD_NODE_H
#define FRAME_HOLD_NODE_H

#include <QPainter>
#include <node_engine.h>

class frame_hold_node : public node_engine
{
public:
	frame_hold_node();
	~frame_hold_node();

	void render(
		QImage *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // FRAME_HOLD_NODE_H
