#ifndef MAKER_HPP
#define MAKER_HPP

#include <QJsonObject>
#include <QString>
#include <QWidget>

#include "../node_graph/node.h"
#include <node_finder.h>
#include <node_view.h>
#include <nodes_load.h>
#include <os.h>
#include <properties.h>
#include <qt.h>
#include <trim_panel.h>
#include <util.h>
#include <viewer.h>

class maker : public QWidget
{
private:
    node_view *_node_view;
    node_finder *finder;
    properties *_properties;
    nodes_load *nodes_loaded;
    QWidget *_vinacomp;

    void setup_shortcut();
    QColor default_color(QString effect_group);

public:
    maker(QWidget *_vinacomp, properties *_properties, nodes_load *_nodes_loaded,
          node_view *_node_view);
    ~maker();

    QString create_fx(QString id);
};

#endif // MAKER_HPP
