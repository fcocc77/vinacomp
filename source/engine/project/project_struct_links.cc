#include <project_struct.h>
#include <util.h>

QStringList project_struct::get_slaves_nodes(node_struct *node) const
{
    QStringList slaves_nodes;

    for (QString param_key : node->params->keys())
    {
        if (!param_key.contains("slaves_nodes"))
            continue;

        QJsonArray param_slaves = node->params->value(param_key).toArray();

        for (QJsonValue slave_value : param_slaves)
        {
            QString slave_node_name = slave_value.toArray().at(0).toString();
            if (!slaves_nodes.contains(slave_node_name))
                slaves_nodes.push_back(slave_node_name);
        }
    }

    return slaves_nodes;
}

void project_struct::unlink_all_knobs(QString node_name)
{
    if (!nodes.contains(node_name))
        return;

    node_struct &node = nodes[node_name];

    for (QString slave_node_name : get_slaves_nodes(&node))
    {
        node_struct &slave_node = nodes[slave_node_name];

        for (QString param_key : slave_node.params->keys())
        {
            if (!param_key.contains("_handler_node"))
                continue;

            QString param_name = param_key;
            param_name.replace("_handler_node", "");

            QString handler_node_name =
                slave_node.params->value(param_key).toArray().at(0).toString();

            if (node.name == handler_node_name)
                unlink_param_from_handler(slave_node_name, param_name);
        }
    }

    for (QString param_key : node.params->keys())
    {
        if (!param_key.contains("_handler_node"))
            continue;

        QString param_name = param_key.replace("_handler_node", "");
        unlink_param_from_handler(node.name, param_name);
    }
}

void project_struct::unlink_param_from_slave(QString handler_node_name,
                                             QString handler_param_name,
                                             QString slave_node_name,
                                             QString slave_param_name)
{
    node_struct &handler_node = nodes[handler_node_name];

    QString param_key = handler_param_name + "_slaves_nodes";

    QJsonArray slave_nodes = handler_node.params->value(param_key).toArray();

    QJsonArray new_slave_nodes;

    for (QJsonValue value : slave_nodes)
    {
        QString _slave_node_name = value.toArray().at(0).toString();
        QString _slave_param_name = value.toArray().at(1).toString();

        if (_slave_node_name == slave_node_name &&
            _slave_param_name == slave_param_name)
            continue;

        new_slave_nodes.push_back(value);
    }

    if (new_slave_nodes.isEmpty())
        handler_node.params->remove(param_key);
    else
        handler_node.params->insert(param_key, new_slave_nodes);
}

bool project_struct::exist_handler_node_in_params(
    QString slave_node_name, QString handler_node_name) const
{
    auto *params = nodes[slave_node_name].params;

    for (QString key : params->keys())
    {
        if (!key.contains("handler_node"))
            continue;

        QString node_name = params->value(key).toArray().at(0).toString();

        if (node_name == handler_node_name)
            return true;
    }

    return false;
}

void project_struct::unlink_param_from_handler(QString slave_node_name,
                                               QString slave_param_name)
{
    if (!nodes.contains(slave_node_name))
        return;

    node_struct &slave_node = nodes[slave_node_name];

    QString param_key = slave_param_name + "_handler_node";
    QJsonArray handler_link = slave_node.params->value(param_key).toArray();

    QString handler_node_name = handler_link.at(0).toString();
    QString handler_param_name = handler_link.at(1).toString();

    slave_node.params->remove(param_key);

    if (!exist_handler_node_in_params(slave_node_name, handler_node_name))
    {
        QJsonArray new_handler_nodes;
        for (QJsonValue value : slave_node.handler_nodes)
        {
            if (value.toString() == handler_node_name)
                continue;
            new_handler_nodes.push_back(value);
        }

        slave_node.handler_nodes = new_handler_nodes;
    }

    unlink_param_from_slave(handler_node_name, handler_param_name,
                            slave_node_name, slave_param_name);
}
