#include <project_struct.h>
#include <util.h>

project_struct::project_struct()
    : frame(0)
{
}

project_struct::~project_struct() {}

void project_struct::insert_node(QString _name, QString _type,
                                 QJsonObject _params, QColor _color,
                                 QPointF _pos, QJsonObject _inputs,
                                 QJsonArray custom_knobs, QString linked,
                                 QSize size, int z_value, QString tips)
{
    if (nodes.contains(_name))
        return;

    QJsonObject *params = new QJsonObject(_params);
    QJsonArray *knobs = new QJsonArray(custom_knobs);

    node_struct node({_name, _color, _type, tips, _pos, params, _inputs, knobs,
                      linked, size, z_value});

    nodes.insert(_name, node);
}

void project_struct::delete_node(QString name)
{
    if (!nodes.contains(name))
        return;

    delete nodes[name].params;
    delete nodes[name].custom_knobs;
    nodes.remove(name);
}

void project_struct::rename_node(QString name, QString new_name)
{
    if (!nodes.contains(name))
        return;

    node_struct aux = nodes[name];

    // no se usa el 'delete_node' porque elimina el puntero de 'params'
    // y solo queremos mover el nodo
    nodes.remove(name);
    nodes.insert(new_name, aux);
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
        if (node.type == "backdrop")
        {
            _node["size"] = QJsonArray({node.size.width(), node.size.height()});
            _node["z_value"] = node.z_value;
        }

        if (!node.custom_knobs->empty())
            _node["knobs"] = *node.custom_knobs;

        if (!node.linked.isEmpty())
            _node["linked"] = node.linked;

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
    load_from_json(jread(project_path));
}

void project_struct::load_from_json(QJsonObject project)
{
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

        QJsonArray custom_knobs = node.value("knobs").toArray();

        QJsonArray size = node.value("size").toArray();
        QSize _size = {size[0].toInt(), size[1].toInt()};

        int z_value = node.value("z_value").toInt();
        QString linked = node.value("linked").toString();

        // extrae el tips del parametro de label que esta el tab 'node'
        QString tips = params.value("label").toString();

        insert_node(name, type, params, _color, _position, inputs, custom_knobs,
                    linked, _size, z_value, tips);
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
