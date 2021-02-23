#ifndef COPY_NODE_H
#define COPY_NODE_H

#include <QPainter>
#include <node_engine.h>

class copy_node : public node_engine
{
public:
	copy_node();
	~copy_node();

	void render(
		cv::Mat *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // COPY_NODE_H
