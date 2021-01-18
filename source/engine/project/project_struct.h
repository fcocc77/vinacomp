#ifndef PROJECT_STRUCT_H
#define PROJECT_STRUCT_H

#include <QJsonObject>
#include <QColor>
#include <QPointF>
#include <QList>
#include <QMap>

#include <util.h>

struct node_struct
{
	QColor color;
	QString type;
	QPointF pos;
	QJsonObject *params;
	QList <QJsonObject> inputs;
};

class project_struct
{
public:
	project_struct();
	~project_struct();

	QMap <QString, node_struct> nodes;

	void insert_node(QString _name, QColor _color, QString _type, QPointF _pos);
	void delete_node(QString name);

	QJsonObject get_project_json() const;

	void save(QString project_path) const;
	void load(QString project_path);
};


#endif // PROJECT_STRUCT_H
