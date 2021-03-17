#ifndef NODES_H
#define NODES_H

#include <QJsonObject>

#include <qt.h>
#include <util.h>

class nodes
{
private:
    QJsonObject *project;

public:
    nodes(QJsonObject *_project);
    ~nodes();

    void create_node(QString node_name);
};

#endif // NODES_H