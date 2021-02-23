#ifndef PREMULT_NODE_H
#define PREMULT_NODE_H

#include <QPainter>
#include <node_engine.h>

class premult_node : public node_engine
{
public:
	premult_node();
	~premult_node();

	void render(
		cv::Mat *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // PREMULT_NODE_H
