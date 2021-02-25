#ifndef PREMULT_NODE_H
#define PREMULT_NODE_H

#include <QPainter>
#include <node_engine.h>

class premult_node : public node_engine
{
public:
	premult_node();
	~premult_node();

	void render( render_data *rdata, QJsonObject *params ) override;
};

#endif // PREMULT_NODE_H
