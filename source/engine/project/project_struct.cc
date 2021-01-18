#include <project_struct.h>

project_struct::project_struct()
{
}

project_struct::~project_struct()
{

}

void project_struct::insert_node(QString _name, QColor _color, QString _type, QPointF _pos)
{
	QJsonObject *params = new QJsonObject();
	node_struct node({ _color, _type, _pos, params, {} });
	nodes.insert(_name, node);
}

void project_struct::delete_node(QString name)
{
	delete nodes[name].params;
	nodes.remove(name);
}

QJsonObject project_struct::get_project_json() const
{
	QJsonObject project;

	QJsonObject _nodes;
	for (QString name : nodes.keys())
	{
		node_struct node = nodes.value(name);

		QJsonObject _node;
		_node["color"] = QJsonArray({node.color.red(), node.color.green(), node.color.blue()});
		_node["type"] = node.type;
		_node["params"] = *node.params;
		_node["pos"] = QJsonArray({node.pos.x(), node.pos.y()});

		_nodes.insert(name, _node);
	}

	project.insert("nodes", _nodes);

	return project;
}

void project_struct::load(QString project_path)
{
	QJsonObject project = jread(project_path);

	QJsonObject _nodes = project.value("nodes").toObject();
	for (QString name : _nodes.keys())
	{
		QJsonObject node = _nodes.value(name).toObject();

		QJsonArray color = node.value("color").toArray();
		QColor _color = {color[0].toDouble(), color[1].toDouble(), color[2].toDouble()};

		QJsonArray position = node.value("pos").toArray();
		QPointF _position = {position[0].toDouble(), position[1].toDouble()};

		QString type = node.value("type").toString();
	}
	print(nodes.value("Blur2").type);
}

void project_struct::save(QString project_path) const
{
	jwrite(project_path, get_project_json());
}
