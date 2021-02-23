#ifndef SATURATION_NODE_H
#define SATURATION_NODE_H

#include <QPainter>
#include <node_engine.h>

class saturation_node : public node_engine
{
public:
	saturation_node();
	~saturation_node();

	void render(
		cv::Mat *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // SATURATION_NODE_H
