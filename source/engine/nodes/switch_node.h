#ifndef SWITCH_NODE_H
#define SWITCH_NODE_H

#include <QPainter>
#include <node_engine.h>

class switch_node : public node_engine
{
public:
	switch_node();
	~switch_node();

	void render(
		QImage *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // SWITCH_NODE_H
