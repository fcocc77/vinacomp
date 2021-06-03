#include <project_struct.h>
#include <util.h>

QString project_struct::replace_parent_name(QString node_name,
                                            QString parent_name,
                                            QString new_parent_name)
{
    if (!node_name.contains(parent_name))
        return node_name;

    return new_parent_name +
           node_name.right(node_name.length() - parent_name.length());
}

QJsonObject project_struct::replace_parent_name_to_params(
    QJsonObject params, QString parent_name, QString new_parent_name)
{
    // Params
    QJsonObject new_params;
    for (QString key : params.keys())
    {
        QJsonValue value = params.value(key);

        if (key.contains("handler_node") || key.contains("slaves_nodes"))
        {
            if (key.contains("slaves_nodes"))
            {
                QJsonArray slaves_nodes;
                for (QJsonValue slave_node_value : value.toArray())
                {
                    QString slave_node_name = slave_node_value.toArray().at(0).toString();
                    QString slave_param_name = slave_node_value.toArray().at(1).toString();

                    slave_node_name = replace_parent_name(
                        slave_node_name, parent_name, new_parent_name);

                    slaves_nodes.push_back(
                        QJsonArray{slave_node_name, slave_param_name});
                }

                new_params.insert(key, slaves_nodes);
            }
            else
            {
                QString handler_node_name = value.toArray().at(0).toString();
                QString handler_parem_name = value.toArray().at(1).toString();

                handler_node_name = replace_parent_name(
                    handler_node_name, parent_name, new_parent_name);

                new_params.insert(
                    key, QJsonArray{handler_node_name, handler_parem_name});
            }
        }
        else
            new_params.insert(key, value);
    }

    return new_params;
}

QJsonArray project_struct::replace_parent_name_to_handlers(
    QJsonArray handler_nodes, QString parent_name, QString new_parent_name)
{

    QJsonArray new_handler_nodes;
    for (QJsonValue value : handler_nodes)
    {
        new_handler_nodes.push_back(replace_parent_name(
            value.toString(), parent_name, new_parent_name));
    }

    return new_handler_nodes;
}

QJsonObject project_struct::replace_parent_name_to_inputs(
    QJsonObject inputs, QString parent_name, QString new_parent_name)
{
    QJsonObject new_inputs;
    for (QString key : inputs.keys())
    {
        QString input = inputs.value(key).toString();
        new_inputs.insert(
            key, replace_parent_name(input, parent_name, new_parent_name));
    }

    return new_inputs;
}

void project_struct::replace_parent_name_to_node(node_struct *node,
                                                 QString parent_name,
                                                 QString new_parent_name,
                                                 bool replace_params)
{
    node->name = replace_parent_name(node->name, parent_name, new_parent_name);

    node->inputs = replace_parent_name_to_inputs(node->inputs, parent_name,
                                                 new_parent_name);

    node->handler_nodes = replace_parent_name_to_handlers(
        node->handler_nodes, parent_name, new_parent_name);

    if (replace_params)
        *node->params = replace_parent_name_to_params(
            *node->params, parent_name, new_parent_name);
}

void project_struct::replace_parent_name_to_children(QString parent_name,
                                                     QString new_parent_name)
{
    for (node_struct child : get_children_nodes(parent_name, true))
    {
        node_struct &_child = nodes[child.name];

        _child.inputs = replace_parent_name_to_inputs(
            _child.inputs, parent_name, new_parent_name);

        _child.handler_nodes = replace_parent_name_to_handlers(
            _child.handler_nodes, parent_name, new_parent_name);

        QString new_node_name =
            replace_parent_name(child.name, parent_name, new_parent_name);

        // aqui se remplazan los links en los 'params'
        rename_node(child.name, new_node_name, false);
    }
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

