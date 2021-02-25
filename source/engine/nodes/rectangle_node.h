#ifndef RECTANGLE_NODE_H
#define RECTANGLE_NODE_H

#include <QPainter>
#include <node_engine.h>

class rectangle_node : public node_engine
{
public:
	rectangle_node();
	~rectangle_node();

	void render(render_data *rdata, QJsonObject *params) override;
};

#endif // RECTANGLE_NODE_H
