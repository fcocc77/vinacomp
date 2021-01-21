#ifndef NODE_ENGINE_H
#define NODE_ENGINE_H

#include <QJsonObject>

#include <util.h>

class node_engine
{
private:
	QJsonObject default_params;

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
protected:
	QJsonValue get(QString key, QJsonObject *params) const;
	void load_default_params(QString node_json_name);
};

#endif // NODE_ENGINE_H
