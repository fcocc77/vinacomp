#ifndef NODE_ENGINE_H
#define NODE_ENGINE_H

#include <QJsonObject>

#include <util.h>

class node_engine
{
public:
	node_engine();
	~node_engine();

	virtual void render(
		QImage *image,
		QJsonObject *params, 
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	);
};

#endif // NODE_ENGINE_H
