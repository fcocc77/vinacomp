#ifndef MERGE_NODE_H
#define MERGE_NODE_H

#include <QPainter>
#include <node_engine.h>

class merge_node : public node_engine
{
public:
	merge_node();
	~merge_node();

	void render(
		cv::Mat *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // MERGE_NODE_H
