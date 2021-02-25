#ifndef UNPREMULT_NODE_H
#define UNPREMULT_NODE_H

#include <QPainter>
#include <node_engine.h>

class unpremult_node : public node_engine
{
public:
	unpremult_node();
	~unpremult_node();

	void render(render_data *rdata, QJsonObject *params) override;
};

#endif // UNPREMULT_NODE_H
