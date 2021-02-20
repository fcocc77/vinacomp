#ifndef KEYER_NODE_H
#define KEYER_NODE_H

#include <QPainter>
#include <node_engine.h>

class keyer_node : public node_engine
{
public:
	keyer_node();
	~keyer_node();

	void render(
		QImage *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // KEYER_NODE_H
