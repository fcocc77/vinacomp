#ifndef GRID_NODE_H
#define GRID_NODE_H

#include <QPainter>
#include <node_engine.h>

class grid_node : public node_engine
{
public:
	grid_node();
	~grid_node();

	void render( render_data *rdata, QJsonObject *params ) override;
};

#endif // GRID_NODE_H
