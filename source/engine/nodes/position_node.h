#ifndef POSITION_NODE_H
#define POSITION_NODE_H

#include <node_engine.h>

#include <opencv2/opencv.hpp>

class position_node : public node_engine
{
public:
	position_node();
	~position_node();

	void render(
		QImage *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // POSITION_NODE_H
