#ifndef RAMP_NODE_H
#define RAMP_NODE_H

#include <QPainter>
#include <node_engine.h>

class ramp_node : public node_engine
{
public:
	ramp_node();
	~ramp_node();

	void render(
		QImage *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // RAMP_NODE_H
