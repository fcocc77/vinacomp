#ifndef SATURATION_NODE_H
#define SATURATION_NODE_H

#include <QPainter>
#include <node_engine.h>

class saturation_node : public node_engine
{
public:
	saturation_node();
	~saturation_node();

	void render(render_data *rdata, QJsonObject *params) override;
};

#endif // SATURATION_NODE_H
