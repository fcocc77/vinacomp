#include <project_struct.h>

project_struct::project_struct()
{
}

project_struct::~project_struct()
{

}

void project_struct::insert_node(QString _name, QColor _color, QString _type, QPointF _pos)
{
	node_struct node({ _color, _type, _pos, {}, {} });
	nodes.insert(_name, node);
}

void project_struct::load_json(QJsonObject project)
{

}

QJsonObject project_struct::get_project_json() const
{

}
