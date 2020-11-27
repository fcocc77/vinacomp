#include <nodes.h>

nodes::nodes(QJsonObject *_project)
    : project(_project)
{
}

nodes::~nodes()
{
}

void nodes::create_node(QString node_name)
{
    QJsonObject node = {};
    node.insert("color", QJsonArray{255, 255, 255});
    node.insert("position", QJsonArray{255, 255});

    qt::insert_json_deep(project, {"nodes", node_name}, node);
}
