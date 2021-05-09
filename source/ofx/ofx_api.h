#ifndef OFX_API_H
#define OFX_API_H

#include <QMap>
#include <QJsonObject>

#include <ofxCore.h>
#include <ofxParam.h>

struct ofx_plugin
{
    OfxPlugin *plugin;
    QString icon;
    QString group_name;
    QString group_icon;
};

struct ofx_binary
{
    QString name;
    QString icon;
};

class ofx_api
{
private:
    QMap<QString, ofx_plugin> plugins;
    QList<ofx_binary> binaries;
    OfxHost *host = new OfxHost;

    void load(QString ofx_file);

public:
    ofx_api();
    QJsonObject get_json_plugin(QString ofx_id) const;
    QList<QJsonObject> get_plugins() const;

    inline QList<ofx_binary> get_binaries() const;
};

inline QList<ofx_binary> ofx_api::get_binaries() const
{
    return binaries;
}

#endif // OFX_API_H
