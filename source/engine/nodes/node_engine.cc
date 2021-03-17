#include <node_engine.h>

node_engine::node_engine() {}

node_engine::~node_engine() {}

void node_engine::render(render_data *rdata, QJsonObject *params) {}

void node_engine::load_default_params(QString node_json_name)
{
    // carga todos los valores por defecto de los parametros,
    // para poder compararlos con los parametros de entrada en 'get()'
    QString node_json = "source/engine/nodes/json/" + node_json_name + ".json";
    for (QJsonValue value : jread(node_json).value("knobs").toArray())
    {
        QJsonObject knob = value.toObject();
        QString key = knob.value("name").toString();
        QJsonValue default_param = knob.value("default");

        default_params.insert(key, default_param);
    }
    //

    // parametros compartidos por todos los nodos
    QString shared_params = "source/engine/nodes/json/shared_params.json";
    for (QJsonValue value : jread(shared_params).value("knobs").toArray())
    {
        QJsonObject knob = value.toObject();
        QString key = knob.value("name").toString();
        QJsonValue default_param = knob.value("default");

        default_params.insert(key, default_param);
    }
}

QJsonValue node_engine::get(QJsonObject *params, QString key) const
{
    // si el valor del parametro entrante no existe, es por que es igual
    // al valor del parametro predeterminado y esta condicional
    // fue realizada en 'trim_panel - setup_knobs()'.
    // Si no existe retorna el valor predeterminado.
    QJsonValue value = params->value(key);
    if (value.isUndefined())
        return default_params.value(key);
    else
        return value;
}

QColor node_engine::get_color(QJsonObject *params, QString key) const
{
    QJsonArray colors = get(params, key).toArray();
    float red = colors[0].toDouble() * 255;
    float green = colors[1].toDouble() * 255;
    float blue = colors[2].toDouble() * 255;
    float alpha = colors[3].toDouble() * 255;

    return QColor(red, green, blue, alpha);
}

QRect node_engine::get_rect(QJsonObject *params, QString key) const
{
    QJsonArray area = get(params, key).toArray();

    int x1 = area[0].toInt();
    int y1 = area[1].toInt();
    int x2 = area[2].toInt();
    int y2 = area[3].toInt();

    return {x1, y1, x2 - x1, y2 - y1};
}
