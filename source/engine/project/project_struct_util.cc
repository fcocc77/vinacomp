#include <project_struct.h>

QString project_struct::replace_parent_name(QString node_name,
                                            QString parent_name,
                                            QString new_parent_name)
{
    if (!node_name.contains('.'))
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

        if (key.contains("linked"))
        {
            if (key.contains("linked_list"))
            {
                QJsonArray linked_list;
                for (QJsonValue linked_list_value : value.toArray())
                {
                    QJsonArray linked;
                    for (QJsonValue linked_value : linked_list_value.toArray())
                    {
                        linked.push_back(
                            replace_parent_name(linked_value.toString(),
                                                parent_name, new_parent_name));
                    }

                    linked_list.push_back(linked);
                }

                new_params.insert(key, linked_list);
            }
            else
            {
                QJsonArray linked;
                for (QJsonValue linked_value : value.toArray())
                    linked.push_back(replace_parent_name(
                        linked_value.toString(), parent_name, new_parent_name));

                new_params.insert(key, linked);
            }
        }
        else
            new_params.insert(key, value);
    }

    return new_params;
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

void project_struct::replace_parent_name_to_children(QString parent_name,
                                                     QString new_parent_name)
{
    auto get_node_name = [=](QString node_name) {
        return replace_parent_name(node_name, parent_name, new_parent_name);
    };

    for (node_struct child : get_children_nodes(parent_name, true))
    {
        node_struct &_child = nodes[child.name];

        _child.inputs = replace_parent_name_to_inputs(child.inputs, parent_name,
                                                      new_parent_name);

        _child.linked = get_node_name(_child.linked);

        *_child.params = replace_parent_name_to_params(
            *_child.params, parent_name, new_parent_name);

        rename_node(child.name, get_node_name(child.name), false);
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

