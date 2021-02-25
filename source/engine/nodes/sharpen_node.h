#ifndef SHARPEN_NODE_H
#define SHARPEN_NODE_H

#include <QPainter>
#include <node_engine.h>

#include <opencv2/opencv.hpp>

class sharpen_node : public node_engine
{
public:
	sharpen_node();
	~sharpen_node();

	void render(render_data *rdata, QJsonObject *params) override;
};

#endif // SHARPEN_NODE_H
