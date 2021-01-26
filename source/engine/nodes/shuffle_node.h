#ifndef SHUFFLE_NODE_H
#define SHUFFLE_NODE_H

#include <QImage>

#include <node_engine.h>

class shuffle_node : public node_engine
{
private:
	void copy(QImage *src, QImage *dst, QList<int> channels_order);
public:
	shuffle_node();
	~shuffle_node();

	void render(
		QImage *image,
		QJsonObject *params, 
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // SHUFFLE_NODE_H
