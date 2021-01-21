#include <node_engine.h>

node_engine::node_engine()
{
}

node_engine::~node_engine()
{
}

void node_engine::render(
	QImage *image,
	QJsonObject *params,
	int frame,
	pair <int, int> &frame_range,
	QRect &bbox)
{
}

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
}

QJsonValue node_engine::get(QString key, QJsonObject *params) const
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
