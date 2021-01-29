#include <shuffle_gui.h>

shuffle_gui::shuffle_gui(QVBoxLayout *controls_layout)
	: dragging(false)
{
	init_connectors();

	this->setObjectName("shuffle_node");
	QHBoxLayout *main_layout = new QHBoxLayout(this);

	// Inputs
	QWidget *input_layer = new QWidget();
	QVBoxLayout *input_layer_layout = new QVBoxLayout(input_layer);
	input_layer_layout->setMargin(0);
	input_layer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	QLabel *input_label = new QLabel("Input Layer");
	input_label->setAlignment(Qt::AlignCenter);
	input_layer_layout->addWidget(input_label);

	in_layer_a = new in_layer("a");
	in_layer_b = new in_layer("b");
	input_layer_layout->addWidget(in_layer_a);
	input_layer_layout->addWidget(in_layer_b);
	//
	//

	// Outputs
	QWidget *output_layer = new QWidget();
	output_layer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	QVBoxLayout *output_layer_layout = new QVBoxLayout(output_layer);
	output_layer_layout->setMargin(0);
	QLabel *output_label = new QLabel("Output Layer");
	output_label->setAlignment(Qt::AlignCenter);
	output_layer_layout->addWidget(output_label);

	out_layer_a = new out_layer("a");
	out_layer_b = new out_layer("b");
	output_layer_layout->addWidget(out_layer_a);
	output_layer_layout->addWidget(out_layer_b);
	//
	//

	connection_viewer = new QWidget();
	connection_viewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	connection_viewer->setObjectName("connection_viewer");

	main_layout->addWidget(input_layer);
	main_layout->addWidget(connection_viewer);
	main_layout->addWidget(output_layer);

	controls_layout->addWidget(this);

	restore_connections();
}

shuffle_gui::~shuffle_gui(){}

void shuffle_gui::restore_connections()
{
	// ! estos datos tienen que venir del proyecto
	QList <int> in_a = {2, 2, 2, -1};
	//

	int red_src = in_a[0];
	int green_src = in_a[1];
	int blue_src = in_a[2];
	int alpha_src = in_a[3];

	connect_channel("a", red_src, "a", 0);
	connect_channel("a", green_src, "a", 1);
	connect_channel("a", blue_src, "a", 2);
	connect_channel("a", alpha_src, "a", 3);

}




void shuffle_gui::changed(QString param_name)
{
	// int x = input_a->x() + input_a->width() + 30;
	// int y = input_a->y() + 50;
}

void shuffle_gui::disconnect_channel(QString in_layer, int in_index, QString out_layer, int out_index)
{
}

void shuffle_gui::set_bw_button(QString layer, int index, bool black, bool white)
{
	out_connector &output_ch = outputs[layer][index];
	QPushButton *white_bt = output_ch.white_button;
	QPushButton *black_bt = output_ch.black_button;

	if (black)
	{
		black_bt->setProperty("connected", true);
		white_bt->setProperty("connected", false);
	}
	else if (white)
	{
		white_bt->setProperty("connected", true);
		black_bt->setProperty("connected", false);
	}
	else
	{
		white_bt->setProperty("connected", false);
		black_bt->setProperty("connected", false);
	}

	// actualiza botones en el stylesheet
	white_bt->style()->unpolish(white_bt);
	white_bt->style()->polish(white_bt);
	white_bt->update();

	black_bt->style()->unpolish(black_bt);
	black_bt->style()->polish(black_bt);
	black_bt->update();
}

void shuffle_gui::connect_channel(QString in_layer, int in_index, QString out_layer, int out_index)
{
	if (in_index == -3)
		return;

	in_connector &input_ch = inputs[in_layer][in_index];
	out_connector &output_ch = outputs[out_layer][out_index];

	// si la entrada conectada es la misma a la actual, retorna
	if (in_index >= 0 && in_index <= 3)
		if (input_ch.connected)
			if (input_ch.output == out_index && input_ch.out_layer == out_layer)
				return;
	//

	disconnect_channel(in_layer, in_index);
	if (out_index < 0 || out_index > 3)
		return;

	// si la entrada es -1 o -2 que son blanco y negro,
	// desconecta la entrada y activa el boton correspondiente
	if (in_index == -2)
	{
		disconnect_channel(in_layer, output_ch.input);
		set_bw_button(out_layer, out_index, false, true);
		return;
	}
	else if (in_index == -1)
	{
		disconnect_channel(in_layer, output_ch.input);
		set_bw_button(out_layer, out_index, true, false);
		return;
	}
	else
	{
		set_bw_button(out_layer, out_index, false, false);
	}
	//
	//

	input_ch.connected = true;
	input_ch.output = out_index;
	input_ch.out_layer = out_layer;

	// desconecta la salida actual del input, ya que en el output no puede
	// haber mas de 1 entrada a la vez
	if (output_ch.connected)
		inputs[output_ch.in_layer][output_ch.input].output = -2;
	//

	//
	output_ch.connected = true;
	output_ch.fill = true;
	output_ch.input = in_index;
	output_ch.in_layer = in_layer;
	//
}

int shuffle_gui::get_output_index(QString layer, QPoint position)
{
	// obtiene el index del conector de salida a partir de la position
	auto &_outputs = outputs[layer];
	int out_index = -1;
	for (int i = 0; i < _outputs.count(); i++)
	{
		out_connector &conn = _outputs[i];
		int x = conn.position.x();
		int y = conn.position.y();
		float distance = qt::distance_points({x, y}, {position.x(), position.y()});
		if (distance < 10)
		{
			out_index = i;
			break;
		}
	}

	return out_index;
}

int shuffle_gui::get_input_index(QString layer, QPoint position)
{
	auto &_inputs = inputs[layer];
	int index = -2;
	for (int i = 0; i < _inputs.count(); i++)
	{
		in_connector &conn = _inputs[i];

		int x = conn.position.x();
		int y = conn.position.y();
		float distance = qt::distance_points({x, y}, {position.x(), position.y()});
		if (distance < 10)
		{
			index = i;
			break;
		}
	}
	return index;
}

void shuffle_gui::resizeEvent(QResizeEvent *event)
{
	update();
}

void shuffle_gui::mouseDoubleClickEvent(QMouseEvent *event)
{
	for (QString layer : {"a", "b"})
	{
		int in_index = get_input_index(layer, event->pos());
		int out_index = get_output_index(layer, event->pos());

		if (in_index == 0 || out_index == 0) connect_channel(layer, 0, layer, 0);
		if (in_index == 1 || out_index == 1) connect_channel(layer, 1, layer, 1);
		if (in_index == 2 || out_index == 2) connect_channel(layer, 2, layer, 2);
		if (in_index == 3 || out_index == 3) connect_channel(layer, 3, layer, 3);
	}
}

void shuffle_gui::mousePressEvent(QMouseEvent *event)
{
	mouse_position = event->pos();

	for (QString layer : {"a", "b"})
	{
		int input_index = get_input_index(layer, event->pos());
		if (input_index >= 0 && input_index <= 3)
		{
			inputs[layer][input_index].dragging = true;
			dragging = true;
		}
	}

	update();
}

void shuffle_gui::mouseReleaseEvent(QMouseEvent *event)
{

	// encuentra la entrada de origen que se esta arrastrando para conectar
	int src_index = -3;
	QString src_layer = "";

	for (QString in_layer : {"a", "b"})
	{
		auto &_inputs = inputs[in_layer];
		for (int i = 0; i < _inputs.count(); i++)
		{
			in_connector &conn = _inputs[i];
			if (conn.dragging)
			{
				src_index = i;
				src_layer = in_layer;
			}

			conn.dragging = false;
		}
	}
	//
	//

	// encuentra la saida de destino
	int dst_index = -1;
	QString dst_layer = "";
	for (QString out_layer : {"a", "b"})
	{
		int out_index = get_output_index(out_layer, event->pos());
		if (out_index >= 0)
		{
			dst_index = out_index;
			dst_layer = out_layer;
			break;
		}
	}
	//
	//

	connect_channel(src_layer, src_index, dst_layer, dst_index);

	dragging = false;

	update();
}

void shuffle_gui::mouseMoveEvent(QMouseEvent *event)
{
	mouse_position = event->pos();

	// si la salida no esta connectada, quita el rellono del circulo
	// a todas las salidas.
	for (QString layer : {"a", "b"})
		for (out_connector &conn : outputs[layer])
			if (!conn.connected)
				conn.fill = false;
	//

	// si el output existe rellena el circulo
	if (dragging)
	{
		for (QString layer : {"a", "b"})
		{
			int out_index = get_output_index(layer, event->pos());
			if (out_index >= 0 && out_index <= 3)
				outputs[layer][out_index].fill = true;
		}
	}
	//

	update();
}
