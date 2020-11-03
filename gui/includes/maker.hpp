#ifndef MAKER_HPP
#define MAKER_HPP

#include <QJsonObject>
#include <QWidget>

#include <util.hpp>
#include <os.hpp>
#include <qt.hpp>
#include <node_graph.hpp>
#include <node_finder.hpp>
#include <properties.hpp>
#include <trim_panel.hpp>

class maker : public QWidget
{
private:
    QJsonObject effects;
    node_graph *_node_graph;
    node_finder *finder;
    properties *_properties;

    void setup_shortcut();
    QColor default_color(QString effect_group);

public:
    maker(node_graph *_node_graph, properties *_properties);
    ~maker();

    void create_fx(QString id);
    QJsonObject get_effect(QString id);
    QJsonObject get_effects(QString group = "");
};

#endif //MAKER_HPP