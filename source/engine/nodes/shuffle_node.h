#ifndef SHUFFLE_NODE_H
#define SHUFFLE_NODE_H

#include <QImage>

#include <node_engine.h>

void isolate_channel(QImage *image, int channel);
void copy(QImage *src, QImage *dst, QList<int> channels_order);

class shuffle_node : public node_engine
{
public:
	shuffle_node();
	~shuffle_node();

	void render(
		cv::Mat *image,
		QJsonObject *params, 
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // SHUFFLE_NODE_H
