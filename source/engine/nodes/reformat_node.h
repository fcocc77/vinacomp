#ifndef REFORMAT_NODE_H
#define REFORMAT_NODE_H

#include <node_engine.h>

class reformat_node : public node_engine
{
public:
	reformat_node();
	~reformat_node();

	void render(
		QImage *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // REFORMAT_NODE_H
