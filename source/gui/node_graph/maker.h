#ifndef MAKER_HPP
#define MAKER_HPP

#include <QJsonObject>
#include <QWidget>
#include <QString>

#include <util.h>
#include <os.h>
#include <qt.h>
#include <node_finder.h>
#include <properties.h>
#include <trim_panel.h>
#include <nodes_load.h>
#include <node_view.h>

class maker : public QWidget
{
private:
    node_view *_node_view;
    node_finder *finder;
    properties *_properties;
    nodes_load *nodes_loaded;

    void setup_shortcut();
    QColor default_color(QString effect_group);

public:
    maker(
        properties *_properties,
        nodes_load *_nodes_loaded,
        node_view *_node_view);
    ~maker();

    QString create_fx(QString id);
};

#endif //MAKER_HPP