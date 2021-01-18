#ifndef PROJECT_STRUCT_H
#define PROJECT_STRUCT_H

#include <QJsonObject>
#include <QColor>
#include <QPointF>
#include <QList>
#include <QMap>

struct node_struct
{
	QColor color;
	QString type;
	QPointF pos;
	QJsonObject params;
	QList <QJsonObject> inputs;
};

class project_struct
{
public:
	project_struct();
	~project_struct();

	QMap <QString, node_struct> nodes;

	void insert_node(QString _name, QColor _color, QString _type, QPointF _pos);

	void load_json(QJsonObject project);
	QJsonObject get_project_json() const;
};


#endif // PROJECT_STRUCT_H
