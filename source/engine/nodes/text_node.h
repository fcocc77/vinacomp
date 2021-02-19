#ifndef TEXT_NODE_H
#define TEXT_NODE_H

#include <QPainter>
#include <node_engine.h>

class text_node : public node_engine
{
public:
	text_node();
	~text_node();

	void render(
		QImage *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // TEXT_NODE_H
