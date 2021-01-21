#ifndef BLUR_NODE_H
#define BLUR_NODE_H

#include <QImage>

#include <node_engine.h>
#include <util.h>

class blur_node : public node_engine
{
public:
	blur_node();
	~blur_node();

	void render(
		QImage *image, 
		QJsonObject *params, 
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // BLUR_NODE_H
