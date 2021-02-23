#ifndef GRADE_NODE_H
#define GRADE_NODE_H

#include <QPainter>
#include <node_engine.h>

#include <opencv2/opencv.hpp>

class grade_node : public node_engine
{
public:
	grade_node();
	~grade_node();

	void render(
		cv::Mat *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // GRADE_NODE_H
