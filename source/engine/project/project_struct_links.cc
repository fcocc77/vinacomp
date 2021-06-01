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

QStringList project_struct::get_handler_nodes(node_struct *node) const
{
    QJsonObject *params = node->params;
    QStringList handler_nodes;

    for (QString param_key : params->keys())
    {
        if (!param_key.contains("handler_node"))
            continue;

        QString handler_node_name =
            params->value(param_key).toArray().at(0).toString();

        if (!handler_nodes.contains(handler_node_name))
            handler_nodes.push_back(handler_node_name);
    }

    return handler_nodes;
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

void project_struct::rename_slave_link(node_struct *handler_node,
                                       QString slave_name,
                                       QString new_slave_name)
{
    QJsonObject *params = handler_node->params;
    QJsonObject new_params;

    for (QString param_key : params->keys())
    {
        if (!param_key.contains("slaves_nodes"))
        {
            new_params.insert(param_key, params->value(param_key));
            continue;
        }

        QJsonArray new_slaves_nodes;

        for (QJsonValue value : params->value(param_key).toArray())
        {
            QString slave_node_name = value.toArray().at(0).toString();
            QString slave_param_name = value.toArray().at(1).toString();

            if (slave_node_name == slave_name)
                slave_node_name = new_slave_name;

            new_slaves_nodes.push_back(
                QJsonArray{slave_node_name, slave_param_name});
        }

        new_params.insert(param_key, new_slaves_nodes);
    }

    *params = new_params;
}

void project_struct::rename_handler_link(node_struct *slave_node,
                                         QString handler_name,
                                         QString new_handler_name)
{
    QJsonObject *params = slave_node->params;
    QJsonObject new_params;

    for (QString param_key : params->keys())
    {
        if (!param_key.contains("handler_node"))
        {
            new_params.insert(param_key, params->value(param_key));
            continue;
        }

        QJsonArray handler_node = params->value(param_key).toArray();

        QString handler_node_name = handler_node.at(0).toString();
        QString handler_param_name = handler_node.at(1).toString();

        if (handler_node_name == handler_name)
            handler_node_name = new_handler_name;

        new_params.insert(param_key,
                          QJsonArray{handler_node_name, handler_param_name});
    }

    *params = new_params;
}

void project_struct::rename_node_links(QString node_name, QString new_name)
{
    node_struct &node = nodes[node_name];

    for (QString handler_node_name : get_handler_nodes(&node))
        rename_slave_link(&nodes[handler_node_name], node_name, new_name);

    for (QString slave_node_name : get_slaves_nodes(&node))
        rename_handler_link(&nodes[slave_node_name], node_name, new_name);
}
