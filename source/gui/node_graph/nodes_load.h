#ifndef NODES_LOAD_H
#define NODES_LOAD_H

#include <QJsonObject>
#include <QObject>
#include <QMap>

#include <ofx_api.h>

struct py_plugin_group
{
    QString name;
    QString icon;
};

class nodes_load : public QObject
{
private:
    ofx_api *ofx;

    QJsonObject effects;
    QMap<QString, py_plugin_group> py_plugins_groups;

public:
    nodes_load();
    ~nodes_load();

    QJsonObject get_effect(QString id) const;
    QJsonObject get_effects(QString group = "") const;
    inline ofx_api *get_ofx() const;

    inline QMap<QString, py_plugin_group> get_py_plugins_groups() const;
};

inline ofx_api *nodes_load::get_ofx() const
{
    return ofx;
}

inline QMap<QString, py_plugin_group> nodes_load::get_py_plugins_groups() const
{
    return py_plugins_groups;
}

#endif // NODES_LOAD_H
