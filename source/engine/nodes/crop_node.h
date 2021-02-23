#ifndef CROP_NODE_H
#define CROP_NODE_H

#include <QPainter>
#include <node_engine.h>

class crop_node : public node_engine
{
public:
	crop_node();
	~crop_node();

	void render(
		cv::Mat *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // CROP_NODE_H
