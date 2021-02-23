#ifndef UNPREMULT_NODE_H
#define UNPREMULT_NODE_H

#include <QPainter>
#include <node_engine.h>

class unpremult_node : public node_engine
{
public:
	unpremult_node();
	~unpremult_node();

	void render(
		cv::Mat *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // UNPREMULT_NODE_H
