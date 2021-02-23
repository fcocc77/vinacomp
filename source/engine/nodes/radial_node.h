#ifndef RADIAL_NODE_H
#define RADIAL_NODE_H

#include <QPainter>
#include <node_engine.h>

class radial_node : public node_engine
{
public:
	radial_node();
	~radial_node();

	void render(
		cv::Mat *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // RADIAL_NODE_H
