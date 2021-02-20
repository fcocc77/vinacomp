#ifndef GRADE_NODE_H
#define GRADE_NODE_H

#include <QPainter>
#include <node_engine.h>

class grade_node : public node_engine
{
public:
	grade_node();
	~grade_node();

	void render(
		QImage *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // GRADE_NODE_H
