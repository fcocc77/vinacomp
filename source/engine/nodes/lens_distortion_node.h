#ifndef LENS_DISTORTION_NODE_H
#define LENS_DISTORTION_NODE_H

#include <QPainter>
#include <node_engine.h>

class lens_distortion_node : public node_engine
{
public:
	lens_distortion_node();
	~lens_distortion_node();

	void render(
		QImage *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // LENS_DISTORTION_NODE_H
