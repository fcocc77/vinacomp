#ifndef TIME_OFFSET_NODE_H
#define TIME_OFFSET_NODE_H

#include <QWidget>
#include <node_engine.h>

class time_offset_node : public node_engine
{
public:
	time_offset_node();
	~time_offset_node();

	void set_offset(QJsonObject *params, int &frame, QString node_name, QWidget *_renderer);
	void render(
		QImage *image,
		QJsonObject *params, 
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // TIME_OFFSET_NODE_H
