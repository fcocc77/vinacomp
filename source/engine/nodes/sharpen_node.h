#ifndef SHARPEN_NODE_H
#define SHARPEN_NODE_H

#include <QPainter>
#include <node_engine.h>

#include <opencv2/opencv.hpp>

class sharpen_node : public node_engine
{
public:
	sharpen_node();
	~sharpen_node();

	void render(
		QImage *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // SHARPEN_NODE_H
