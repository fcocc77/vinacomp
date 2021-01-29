#include <shuffle_gui.h>

connector::connector(QString _layer, QColor _color)
	: layer(_layer)
	, color(_color)
	, connected(false)
{ }

QColor connector::get_color() const
{
	return color;
}

QPoint connector::get_position() const
{
	return position;
}

void connector::set_position(QPoint _position)
{
	position = _position;
}

bool connector::is_connected() const
{
	return connected;
}

void connector::set_connected(bool _connected)
{
	connected = _connected;
}

out_connector::out_connector(QString _layer, QString label, QColor _color)
	: connector(_layer, _color)
{
	this->setObjectName("output");
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	layout->setMargin(0);

	QLabel *channel = new QLabel(label);
	channel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	black_button = new QPushButton();
	connect(black_button, &QPushButton::clicked, this, [=]() {
		// connect_channel(layer, -1, layer, index);
		// update();
	});
	black_button->setProperty("connected", false);
	black_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	black_button->setObjectName("cblack");

	white_button = new QPushButton();
	connect(white_button, &QPushButton::clicked, this, [=]() {
		// connect_channel(layer, -2, layer, index);
		// update();
	});
	white_button->setProperty("connected", false);
	white_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	white_button->setObjectName("cwhite");

	layout->addWidget(black_button);
	layout->addWidget(white_button);
	layout->addWidget(channel);

}

out_connector::~out_connector() { }


void out_connector::connect_input(connector *_in_conn)
{
	// ya que las salidas solo pueden tener una entrada conectada,
	// si es que la salida esta conectada, la desconecta antes de conectar la otra
	disconnect();
	//

	in_conn = _in_conn;
	set_connected(true);
}

void out_connector::disconnect()
{
	if (!is_connected())
		return;

	in_connector *_in_conn = dynamic_cast<in_connector*>(in_conn);
	_in_conn->disconnect(this);

	in_conn = nullptr;
	set_connected(false);
}

in_connector::in_connector(QString _layer, QString label, QColor _color)
	: connector(_layer, _color)
{
	this->setObjectName("input");
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	layout->setMargin(0);

	QLabel *channel_label = new QLabel(label);
	channel_label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	layout->addWidget(channel_label);
}

void in_connector::connect_output(out_connector *out_conn)
{
	if (!outputs.contains(out_conn))
	{
		out_conn->connect_input(this);
		outputs.push_back(out_conn);
	}

	connected = outputs.count() > 0;
}

void in_connector::disconnect(out_connector *out_conn)
{
	outputs.removeOne(out_conn);
	connected = outputs.count() > 0;
}

QList <out_connector*> in_connector::get_outputs() const
{
	return outputs;
}

in_connector::~in_connector() { }

in_layer::in_layer(QString layer)
{
	this->setMaximumWidth(200);
	this->setMinimumWidth(200);
	this->setObjectName("input_channels");
	QVBoxLayout *input_layout = new QVBoxLayout(this);

	// ! hay que poner las entradas disponible que tiene la imagen
	QWidget *menus = new QWidget();
	menus->setObjectName("inputs_menu");
	QHBoxLayout *menus_layout = new QHBoxLayout(menus);
	menus_layout->setMargin(0);

	combo_box *layer_box = new combo_box({
		{"rgba", "rgba"},
		{"depth", "depth"}
	});
	combo_box *node_input = new combo_box({
		{"B", "b"},
		{"A", "a"}
	});
	layer_box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	menus_layout->addWidget(node_input);
	menus_layout->addWidget(layer_box);

	QColor red_color = {200, 50, 50};
	QColor green_color = {50, 200, 50};
	QColor blue_color = {30, 70, 255};
	QColor white_color = {200, 200, 200};

	red_connector = new in_connector(layer, "rgba.red", red_color);
	green_connector = new in_connector(layer, "rgba.green", green_color);
	blue_connector = new in_connector(layer, "rgba.blue", blue_color);
	alpha_connector = new in_connector(layer, "rgba.alhpa", white_color);

	input_layout->addWidget(menus);
	input_layout->addWidget(red_connector);
	input_layout->addWidget(green_connector);
	input_layout->addWidget(blue_connector);
	input_layout->addWidget(alpha_connector);
}

in_layer::~in_layer() { }

QList<in_connector*> in_layer::get_connectors() const
{
	return {red_connector, green_connector, blue_connector, alpha_connector};
}

out_layer::out_layer(QString layer)
{
	this->setMaximumWidth(200);
	this->setMinimumWidth(200);
	this->setObjectName("output_channels");
	QVBoxLayout *output_layout = new QVBoxLayout(this);

	// ! hay que poner las entradas disponible que tiene la imagen
	combo_box *layer_box = new combo_box({
		{"rgba", "rgba"},
		{"depth", "depth"}
	});
	layer_box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	QColor red_color = {200, 50, 50};
	QColor green_color = {50, 200, 50};
	QColor blue_color = {30, 70, 255};
	QColor white_color = {200, 200, 200};

	red_connector = new out_connector(layer, "rgba.red", red_color);
	green_connector = new out_connector(layer, "rgba.green", green_color);
	blue_connector = new out_connector(layer, "rgba.blue", blue_color);
	alpha_connector = new out_connector(layer, "rgba.alpha", white_color);

	output_layout->addWidget(layer_box);
	output_layout->addWidget(red_connector);
	output_layout->addWidget(green_connector);
	output_layout->addWidget(blue_connector);
	output_layout->addWidget(alpha_connector);
}

out_layer::~out_layer() { }

QList<out_connector*> out_layer::get_connectors() const
{
	return {red_connector, green_connector, blue_connector, alpha_connector};
}
