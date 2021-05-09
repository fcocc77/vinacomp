#ifndef NODES_LOAD_H
#define NODES_LOAD_H

#include <QJsonObject>
#include <QObject>
#include <QMap>

#include <ofx_api.h>

class nodes_load : public QObject
{
private:
    ofx_api *ofx;

    QJsonObject effects;
public:
    nodes_load();
    ~nodes_load();

    QJsonObject get_effect(QString id) const;
    QJsonObject get_effects(QString group = "") const;
    inline ofx_api *get_ofx() const;
};

inline ofx_api *nodes_load::get_ofx() const
{
    return ofx;
}

#endif // NODES_LOAD_H
