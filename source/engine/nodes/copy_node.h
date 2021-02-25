#ifndef COPY_NODE_H
#define COPY_NODE_H

#include <QPainter>
#include <node_engine.h>

class copy_node : public node_engine
{
public:
	copy_node();
	~copy_node();

	void render( render_data *rdata, QJsonObject *params ) override;
};

#endif // COPY_NODE_H
