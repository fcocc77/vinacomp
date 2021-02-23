#ifndef FRAME_RANGE_NODE_H
#define FRAME_RANGE_NODE_H

#include <node_engine.h>

class frame_range_node : public node_engine
{
public:
	frame_range_node();
	~frame_range_node();

	void render(
		cv::Mat *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox) override;

	pair <int, int> get_frame_range(QJsonObject *params) const;
};

#endif // FRAME_RANGE_NODE_H
