#ifndef MIRROR_NODE_H
#define MIRROR_NODE_H

#include <QPainter>
#include <node_engine.h>

void flip_flop(cv::Mat *image, bool flip = false, bool flop = false);

class mirror_node : public node_engine
{
public:
	mirror_node();
	~mirror_node();

	void render(
		cv::Mat *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // MIRROR_NODE_H
