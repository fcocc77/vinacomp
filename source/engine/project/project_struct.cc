#include <project_struct.h>

project_struct::project_struct() {}

project_struct::~project_struct() {}

void project_struct::insert_node(QString _name, QColor _color, QString _type, QPointF _pos,
                                 QJsonObject _params, QJsonObject _inputs)
{
    if (nodes.contains(_name))
        return;

    QJsonObject *params = new QJsonObject(_params);
    node_struct node({_color, _type, _pos, params, _inputs});
    nodes.insert(_name, node);
}

void project_struct::delete_node(QString name)
{
    if (nodes.contains(name))
        return;

    delete nodes[name].params;
    nodes.remove(name);
}

void project_struct::insert_input(QString src_node, QString dst_node, int index)
{
    QString key = "in" + QString::number(index);
    nodes[src_node].inputs.insert(key, dst_node);
}

void project_struct::delete_input(QString node, int index)
{
    QString key = "in" + QString::number(index);
    nodes[node].inputs.remove(key);
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
        _node["inputs"] = node.inputs;
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
        QJsonObject params = node.value("params").toObject();
        QJsonObject inputs = node.value("inputs").toObject();

        insert_node(name, _color, type, _position, params, inputs);
    }
}

void project_struct::save(QString project_path) const
{
    jwrite(project_path, get_project_json());
}
