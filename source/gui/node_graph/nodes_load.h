#ifndef NODES_LOAD_H
#define NODES_LOAD_H

#include <QJsonObject>
#include <QObject>
#include <QMap>

class nodes_load : public QObject
{
private:
    QMap<QString, QString> ofx_list;

    QJsonObject effects;
    void load_ofx_plugins();
    void load_ofx(QString ofx_name);

public:
    nodes_load();
    ~nodes_load();

    QJsonObject get_effect(QString id) const;
    QJsonObject get_effects(QString group = "") const;
    inline QMap<QString, QString> get_ofx_list() const;
};

inline QMap<QString, QString> nodes_load::get_ofx_list() const
{
    return ofx_list;
}

#endif // NODES_LOAD_H
