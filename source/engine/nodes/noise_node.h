#ifndef NOISE_NODE_H
#define NOISE_NODE_H

#include <QPainter>
#include <node_engine.h>

class noise_node : public node_engine
{
public:
	noise_node();
	~noise_node();

	void render(
		QImage *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // NOISE_NODE_H
