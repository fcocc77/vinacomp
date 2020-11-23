#ifndef NODES_LOAD_H
#define NODES_LOAD_H

#include <QObject>
#include <QJsonObject>

#include <os.h>

class nodes_load : public QObject
{
private:
    QJsonObject effects;

public:
    nodes_load();
    ~nodes_load();

    QJsonObject get_effect(QString id) const;
    QJsonObject get_effects(QString group = "") const;
};

#endif // NODES_LOAD_H