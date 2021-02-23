#ifndef ERODE_NODE_H
#define ERODE_NODE_H

#include <QPainter>
#include <node_engine.h>

class erode_node : public node_engine
{
public:
	erode_node();
	~erode_node();

	void render(
		cv::Mat *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // ERODE_NODE_H
