#ifndef TRANSFORM_NODE_H
#define TRANSFORM_NODE_H

#include <QPainter>
#include <node_engine.h>

class transform_node : public node_engine
{
public:
	transform_node();
	~transform_node();

	void render(
		QImage *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // TRANSFORM_NODE_H
