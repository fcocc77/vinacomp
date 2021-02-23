#ifndef IDISTORT_NODE_H
#define IDISTORT_NODE_H

#include <QPainter>
#include <node_engine.h>

class idistort_node : public node_engine
{
public:
	idistort_node();
	~idistort_node();

	void render(
		cv::Mat *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // IDISTORT_NODE_H
