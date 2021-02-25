#ifndef FRAME_HOLD_NODE_H
#define FRAME_HOLD_NODE_H

#include <QPainter>
#include <node_engine.h>

class frame_hold_node : public node_engine
{
public:
	frame_hold_node();
	~frame_hold_node();

	void render( render_data *rdata, QJsonObject *params ) override;
};

#endif // FRAME_HOLD_NODE_H
