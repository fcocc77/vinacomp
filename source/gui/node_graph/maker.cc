#include <maker.h>
#include <panels_layout.h>
#include <vinacomp.h>
#include <os.h>
#include <util.h>
#include <qt.h>
#include "../node_graph/node.h"
#include <trim_panel.h>
#include <viewer.h>

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
    connect(finder->tree, &QTreeWidget::itemPressed, this, [this](QTreeWidgetItem *item) {
        QString node_id = item->text(1);
        create_fx(node_id);
        finder->hide();
    });

    setup_shortcut();
}

maker::~maker() {}

void maker::setup_shortcut()
{
    qt::shortcut("Tab", _node_view, [this]() { finder->show_finder(); });

    qt::shortcut("Escape", _node_view, [this]() { finder->hide(); });
}

QString maker::create_fx(QString id)
{
    QJsonObject effect = nodes_loaded->get_effect(id);
    if (effect.empty())
        return 0;

    QString group = effect["group"].toString();
    QString label = effect["label"].toString();
    QColor color = default_color(group);

    // Encuentra un nombre disponible
    QString name;
    int node_number = 1;
    while (true)
    {
        name = label + QString::number(node_number);
        if (!_node_view->get_node(name))
            break;
        node_number++;
    }
    //
    //

    // si el node_graph es un grupo, le antepone al nombre el nombre del grupo
    node_graph *__node_graph = static_cast<node_graph *>(_node_graph);
    if (__node_graph->is_group())
        name = __node_graph->get_group_name() + '.' + name;
    //

    // Creación del nodo, con un nombre que no se ha utilizado.
    node *_node = _node_view->create_node(name, color, id);
    _node->make_panel();
    //
    //

    return name;
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
