#ifndef MAKER_HPP
#define MAKER_HPP

#include <QJsonObject>
#include <QString>
#include <QWidget>

#include <node_finder.h>
#include <node_view.h>
#include <nodes_load.h>
#include <properties.h>

class maker : public QWidget
{
private:
    node_view *_node_view;
    QWidget *_node_graph;
    node_finder *finder;
    properties *_properties;
    nodes_load *nodes_loaded;
    QWidget *_vinacomp;

    QColor default_color(QString effect_group);

public:
    maker(QWidget *_vinacomp, properties *_properties,
          nodes_load *_nodes_loaded, node_view *_node_view,
          QWidget *_node_graph);
    ~maker();

    QString create_fx(QString id);
    inline node_finder *get_finder() const;
};

inline node_finder *maker::get_finder() const
{
    return finder;
}

#endif // MAKER_HPP
