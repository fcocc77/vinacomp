#ifndef INVERT_NODE_H
#define INVERT_NODE_H

#include <node_engine.h>

class invert_node : public node_engine
{
public:
	invert_node();
	~invert_node();

	void render(
		cv::Mat *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // INVERT_NODE_H
