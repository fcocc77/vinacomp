#include <maker.hpp>

maker::maker(/* args */)
{

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

    create_fx("grsade");
}

maker::~maker()
{
}

void maker::create_fx(QString type)
{
    QJsonObject effect = get_effect(type);
    print(effect);
}

QJsonObject maker::get_effect(QString type)
{
    return effects.value(type).toObject();
}

QJsonObject maker::get_effects()
{
    return effects;
}