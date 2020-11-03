#ifndef MAKER_HPP
#define MAKER_HPP

#include <QJsonObject>
#include <QWidget>

#include <util.hpp>
#include <os.hpp>
#include <qt.hpp>
#include <node_graph.hpp>
#include <node_finder.hpp>

class maker : public QWidget
{
private:
    QJsonObject effects;
    node_graph *_node_graph;
    node_finder *finder;

    void setup_shortcut();
    QColor default_color(QString effect_group);

public:
    maker(node_graph *_node_graph);
    ~maker();

    void create_fx(QString id);
    QJsonObject get_effect(QString id);
    QJsonObject get_effects();
};

#endif //MAKER_HPP