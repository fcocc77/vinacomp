#include <project_struct.h>
#include <util.h>

project_struct::project_struct()
    : frame(0)
{
}

project_struct::~project_struct() {}

void project_struct::insert_node(QString _name, QColor _color, QString _type, QPointF _pos,
                                 QJsonObject _params, QJsonObject _inputs)
{
    if (nodes.contains(_name))
        return;

    QJsonObject *params = new QJsonObject(_params);
    node_struct node({_name, _color, _type, _pos, params, _inputs});
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

    // Nodos a json
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
    //

    // Layers a json
    QJsonObject _global;
    QJsonArray layers;
    for (auto layer : global.layers)
    {
        layers.push_back(
            {{layer.name, layer.red, layer.green, layer.blue, layer.alpha}});
    }
    _global.insert("layers", layers);
    //

    // NodeView Rect
    QRectF rect = global.node_view_rect;
    QJsonArray _rect = {rect.x(), rect.y(), rect.width(), rect.height()};
    _global.insert("node_view_rect", _rect);
    //

    project.insert("global", _global);
    project.insert("nodes", _nodes);

    return project;
}

void project_struct::load(QString project_path)
{
    QJsonObject project = jread(project_path);

    // Nodos
    QJsonObject _nodes = project.value("nodes").toObject();
    for (QString name : _nodes.keys())
    {
        QJsonObject node = _nodes.value(name).toObject();

        QJsonArray color = node.value("color").toArray();
        QColor _color = {color[0].toInt(), color[1].toInt(), color[2].toInt()};

        QJsonArray position = node.value("pos").toArray();
        QPointF _position = {position[0].toDouble(), position[1].toDouble()};

        QString type = node.value("type").toString();
        QJsonObject params = node.value("params").toObject();
        QJsonObject inputs = node.value("inputs").toObject();

        insert_node(name, _color, type, _position, params, inputs);
    }
    //

    // Layers
    QJsonObject _global = project.value("global").toObject();
    QJsonArray _layers = _global.value("layers").toArray();
    for (QJsonValue value : _layers)
    {
        QJsonArray _layer = value.toArray();

        layer_struct layer;

        layer.name = _layer[0].toString();
        layer.red = _layer[1].toBool();
        layer.green = _layer[2].toBool();
        layer.blue = _layer[3].toBool();
        layer.alpha = _layer[4].toBool();

        global.layers.push_back(layer);
    }
    //
    

    // NodeView Rect
    QJsonArray _rect = _global.value("node_view_rect").toArray();
    global.node_view_rect.setRect(_rect[0].toDouble(), _rect[1].toDouble(),
                                  _rect[2].toDouble(), _rect[3].toDouble());
    //
}

void project_struct::save(QString project_path) const
{
    jwrite(project_path, get_project_json());
}

QJsonValue project_struct::get_value_frame(QJsonObject *params, QString param_name, int frame)
{
    return params->value(param_name)
        .toObject()
        .value("value")
        .toObject()
        .value("f" + QString::number(frame))
        .toArray()[0];
}
