#ifndef CLAMP_NODE_H
#define CLAMP_NODE_H

#include <QPainter>
#include <node_engine.h>

class clamp_node : public node_engine
{
public:
	clamp_node();
	~clamp_node();

	void render(
		QImage *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // CLAMP_NODE_H
