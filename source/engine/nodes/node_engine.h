#ifndef NODE_ENGINE_H
#define NODE_ENGINE_H

#include <QJsonObject>

#include <util.h>

class node_engine
{
public:
	node_engine();
	~node_engine();

	virtual QImage *render(QImage *image, QJsonObject *params, int frame);
};

#endif // NODE_ENGINE_H
