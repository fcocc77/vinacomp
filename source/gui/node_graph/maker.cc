#include "../node_graph/node.h"
#include <maker.h>
#include <node_viewer.h>
#include <os.h>
#include <panels_layout.h>
#include <project_struct.h>
#include <qt.h>
#include <trim_panel.h>
#include <util.h>
#include <viewer.h>
#include <vinacomp.h>

maker::maker(QWidget *__vinacomp, properties *__properties,
             nodes_load *_nodes_loaded, node_view *__node_view,
             QWidget *__node_graph)

    : _node_view(__node_view)
    , _node_graph(__node_graph)
    , _properties(__properties)
    , nodes_loaded(_nodes_loaded)
    , _vinacomp(__vinacomp)
{

    finder = new node_finder(_node_view, nodes_loaded);
    connect(finder, &node_finder::created, this,
            [this](QString id) { create_fx(id); });
}

maker::~maker() {}

QString maker::get_available_name(node_view *_node_view, QString name)
{
    QString available_name = name + "1";

    int number = 0;
    while (true)
    {
        if (!_node_view->get_node(available_name))
            break;

        number++;
        available_name = name + QString::number(number);
    }

    return available_name;
}

node *maker::create_fx(QString id, bool basic_creation)
{
    QJsonObject effect = nodes_loaded->get_effect(id);
    if (effect.empty())
        return 0;

    QString group = effect["group"].toString();
    QString label = effect["label"].toString();
    QColor color = default_color(group);
    QString name = label;

    // si el node_graph es un grupo, le antepone al nombre el nombre del grupo
    node_graph *__node_graph = static_cast<node_graph *>(_node_graph);
    if (__node_graph->is_group())
        name = __node_graph->get_group_name() + '.' + name;
    //

    // Creación del nodo, con un nombre que no se ha utilizado.
    node_struct node_data;
    node_data.name = get_available_name(_node_view, name);
    node_data.color = color;
    node_data.type = id;
    node_data.params = nullptr;

    node *_node = _node_view->create_node(node_data, basic_creation);
    if (id != "backdrop")
        _node->make_panel();

    node_viewer *_node_viewer = dynamic_cast<node_viewer *>(_node);
    if (_node_viewer)
        _node_viewer->make_viewer();
    //

    return _node;
}

QColor maker::default_color(QString effect_group)
{
    QMap<QString, QColor> colors;

    colors.insert("draw", QColor(76, 128, 51));
    colors.insert("time", QColor(175, 163, 93));
    colors.insert("channel", QColor(158, 59, 98));
    colors.insert("color", QColor(122, 168, 255));
    colors.insert("filter", QColor(204, 128, 77));
    colors.insert("keyer", QColor(0, 200, 0));
    colors.insert("merge", QColor(75, 93, 198));
    colors.insert("transform", QColor(165, 121, 170));

    if (colors.contains(effect_group))
        return colors.value(effect_group);
    else
        return QColor(150, 150, 150);
}
