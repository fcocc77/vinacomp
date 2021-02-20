#ifndef DISSOLVE_NODE_H
#define DISSOLVE_NODE_H

#include <QPainter>
#include <node_engine.h>

class dissolve_node : public node_engine
{
public:
	dissolve_node();
	~dissolve_node();

	void render(
		QImage *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // DISSOLVE_NODE_H
