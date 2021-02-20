#ifndef CORNEL_PIN_NODE_H
#define CORNEL_PIN_NODE_H

#include <node_engine.h>

#include <opencv2/opencv.hpp>

class cornel_pin_node : public node_engine
{
public:
	cornel_pin_node();
	~cornel_pin_node();

	void render(
		QImage *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // CORNEL_PIN_NODE_H
