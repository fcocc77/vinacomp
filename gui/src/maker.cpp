#include <maker.hpp>

maker::maker(node_graph *__node_graph)
{
    _node_graph = __node_graph;

    QString json_nodes_path = "engine/nodes/json";

    // Carga todos los efectos del directorio
    for (QString effect_path : os::listdir(json_nodes_path))
    {
        QJsonObject effect = jread(effect_path);
        QString effect_type = effect.value("type").toString();

        effects.insert(effect_type, effect);
    }
    //
    //

    finder = new node_finder(_node_graph);
    setup_shortcut();
}

maker::~maker()
{
}

void maker::setup_shortcut()
{
    qt::shortcut("Tab", _node_graph, [this]() {
        finder->show_finder();
    });

    qt::shortcut("Escape", _node_graph, [this]() {
        finder->hide();
    });

    qt::shortcut("G", _node_graph, [this]() {
        create_fx("grade");
    });

    qt::shortcut("T", _node_graph, [this]() {
        create_fx("transform");
    });

    qt::shortcut("B", _node_graph, [this]() {
        create_fx("blur");
    });
}

void maker::create_fx(QString type)
{
    QJsonObject effect = this->get_effect(type);
    if (effect.empty())
        return;

    QString label = effect["label"].toString();
    QString icon_name = effect["icon"].toString();

    // Creación del nodo, con un número que no se ha utilizado.
    int node_number = 1;
    while (true)
    {
        QString name = label + QString::number(node_number);
        if (!_node_graph->get_node(name))
        {
            _node_graph->create_node(name, icon_name);
            break;
        }

        node_number++;
    }
    //
    //
}

QJsonObject maker::get_effect(QString type)
{
    return effects.value(type).toObject();
}

QJsonObject maker::get_effects()
{
    return effects;
}

QColor maker::default_color(QString effect_name)
{
}
