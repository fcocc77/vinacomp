
#ifndef CONSTANT_NODE_H
#define CONSTANT_NODE_H

#include <node_engine.h>

class constant_node : public node_engine
{
public:
	constant_node();
	~constant_node();

	void render(
		QImage *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // CONSTANT_NODE_H
