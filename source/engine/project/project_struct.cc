#include <project_struct.h>
#include <util.h>
#include <global.h>

project_struct::project_struct()
    : frame(0)
{
}

project_struct::~project_struct() {}

void project_struct::insert_node(node_struct node, QJsonObject _params,
                                 QJsonArray custom_knobs)
{
    if (nodes.contains(node.name))
        return;

    node.params = new QJsonObject(_params);
    node.custom_knobs = new QJsonArray(custom_knobs);

    nodes.insert(node.name, node);

    if (node.type == "group")
        create_base_children_for_group(node);

    if (node.plugin)
        create_children_plugin(node);
}

void project_struct::create_base_children_for_group(node_struct node)
{
    node_struct input_props;
    input_props.type = "input";
    input_props.name = node.name + ".Input";
    input_props.color = node.color;
    input_props.pos = {0, 0};

    node_struct output_props;
    output_props.type = "output";
    output_props.name = node.name + ".Output";
    output_props.color = node.color;
    output_props.pos = {0, 200};

    insert_node(input_props, {});
    insert_node(output_props, {});
}

void project_struct::create_children_plugin(node_struct node)
{
    QJsonObject plugin_nodes =
        jread(PY_PLUGINS_PATH + '/' + node.type + ".json")
            .value("nodes")
            .toObject();

    for (QString relative_name : plugin_nodes.keys())
    {
        QString new_name = node.name + '.' + relative_name;
        QJsonObject node_obj = plugin_nodes.value(relative_name).toObject();

        insert_node(get_node_from_object(new_name, node_obj));
    }
}

QList<node_struct> project_struct::get_children_nodes(QString parent_name,
                                                      bool recursive) const
{
    QList<node_struct> children;
    parent_name += ".";

    for (node_struct node : nodes)
    {
        if (recursive)
        {
            if (node.name.indexOf(parent_name) == 0)
                children.push_back(node);
        }
        else
        {
            QString parent_of_node =
                node.name.left(node.name.lastIndexOf('.') + 1);

            if (parent_of_node == parent_name)
                children.push_back(node);
        }
    }

    return children;
}

void project_struct::delete_node(QString name)
{
    // ! borrar aqui los hijos de los nodos si es que tienen
    if (!nodes.contains(name))
        return;

    node_struct node = nodes.value(name);

    if (node.type == "group" || node.plugin)
        for (node_struct child_node : get_children_nodes(node.name))
            delete_node(child_node.name);

    delete nodes[name].params;
    delete nodes[name].custom_knobs;
    nodes.remove(name);
}

void project_struct::replace_parent_name_to_children(QString parent_name,
                                                     QString new_parent_name)
{
    for (node_struct child : get_children_nodes(parent_name, true))
    {
        QString new_child_name =
            new_parent_name +
            child.name.right(child.name.length() - child.name.indexOf('.', 1));

        rename_node(child.name, new_child_name, false);
    }
}

void project_struct::rename_node(QString name, QString new_name,
                                 bool rename_children)
{
    if (!nodes.contains(name))
        return;

    node_struct aux = nodes[name];

    if (rename_children && (aux.type == "group" || aux.plugin))
        replace_parent_name_to_children(name, new_name);

    aux.name = new_name;

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

        if (!node.script.isEmpty())
            _node["script"] = node.script;

        if (node.plugin)
            _node["plugin"] = node.plugin;

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

node_struct project_struct::get_node_from_object(QString name,
                                                 QJsonObject node_obj) const
{
    QJsonArray color = node_obj.value("color").toArray();
    QJsonArray position = node_obj.value("pos").toArray();
    QJsonObject params = node_obj.value("params").toObject();
    QJsonArray custom_knobs = node_obj.value("knobs").toArray();
    QJsonArray size = node_obj.value("size").toArray();

    node_struct node;

    node.name = name;
    node.type = node_obj.value("type").toString();
    node.color = {color[0].toInt(), color[1].toInt(), color[2].toInt()};
    node.pos = {position[0].toDouble(), position[1].toDouble()};
    node.inputs = node_obj.value("inputs").toObject();
    node.size = {size[0].toInt(), size[1].toInt()};
    node.z_value = node_obj.value("z_value").toInt();
    node.linked = node_obj.value("linked").toString();
    node.script = node_obj.value("script").toString();
    node.plugin = node_obj.value("plugin").toBool();

    // extrae el tips del parametro de label que esta el tab 'node'
    node.tips = params.value("label").toString();

    return node;
}

void project_struct::load_from_json(QJsonObject project)
{
    // Nodos
    QJsonObject _nodes = project.value("nodes").toObject();
    for (QString name : _nodes.keys())
    {
        QJsonObject node_obj = _nodes.value(name).toObject();

        QJsonObject params = node_obj.value("params").toObject();
        QJsonArray custom_knobs = node_obj.value("knobs").toArray();

        insert_node(get_node_from_object(name, node_obj), params, custom_knobs);
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

QList<node_struct>
project_struct::get_nodes_from_group(QString group_name) const
{
    if (group_name.isEmpty())
    {
        // crea lista con los nodos principales ya que no son grupos al no tener
        // punto
        QList<node_struct> main_nodes;
        for (QString name : nodes.keys())
        {
            auto node = nodes.value(name);
            if (!name.contains('.'))
                main_nodes.push_back(node);
        }

        return main_nodes;
    }

    return get_children_nodes(group_name);
}
