#include <shuffle_gui.h>

shuffle_gui::shuffle_gui(QVBoxLayout *controls_layout)
	: dragging_input(nullptr)
	, dragging(false)
{
	// init_connectors();

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

	connection_viewer = new QWidget();
	connection_viewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	connection_viewer->setObjectName("connection_viewer");

	main_layout->addWidget(input_layer);
	main_layout->addWidget(connection_viewer);
	main_layout->addWidget(output_layer);

	controls_layout->addWidget(this);

	// restore_connections();
}

shuffle_gui::~shuffle_gui(){}

in_connector *shuffle_gui::get_in_connector(QPoint position) const
{
	auto get_connector = [=](in_layer *layer)
	{
		in_connector *_conn = nullptr;
		for (in_connector *conn : layer->get_connectors())
		{
			int x = conn->get_position().x();
			int y = conn->get_position().y();
			float distance = qt::distance_points({x, y}, {position.x(), position.y()});
			if (distance < 10)
			{
				_conn = conn;
				break;
			}
		}

		return _conn;
	};

	in_connector *conn_a = get_connector(in_layer_a);
	in_connector *conn_b = get_connector(in_layer_b);

	if (conn_a) return conn_a;
	else return conn_b;
}

out_connector *shuffle_gui::get_out_connector(QPoint position) const
{
	auto get_connector = [=](out_layer *layer)
	{
		out_connector *_conn = nullptr;
		for (out_connector *conn : layer->get_connectors())
		{
			int x = conn->get_position().x();
			int y = conn->get_position().y();
			float distance = qt::distance_points({x, y}, {position.x(), position.y()});
			if (distance < 10)
			{
				_conn = conn;
				break;
			}
		}
		return _conn;
	};

	out_connector *conn_a = get_connector(out_layer_a);
	out_connector *conn_b = get_connector(out_layer_b);

	if (conn_a) return conn_a;
	else return conn_b;
}

void shuffle_gui::mousePressEvent(QMouseEvent *event)
{
	mouse_position = event->pos();

	dragging_input = get_in_connector(event->pos());
	if (dragging_input)
		dragging = true;

	update();
}

void shuffle_gui::mouseReleaseEvent(QMouseEvent *event)
{
	out_connector *out_conn = get_out_connector(event->pos());
	if (dragging_input && out_conn)
		dragging_input->connect_output(out_conn);

	dragging_input = nullptr;
	dragging = false;

	// // encuentra la entrada de origen que se esta arrastrando para conectar
	// int src_index = -3;
	// QString src_layer = "";

	// for (QString in_layer : {"a", "b"})
	// {
		// auto &_inputs = inputs[in_layer];
		// for (int i = 0; i < _inputs.count(); i++)
		// {
			// in_connector &conn = _inputs[i];
			// if (conn.dragging)
			// {
				// src_index = i;
				// src_layer = in_layer;
			// }

			// conn.dragging = false;
		// }
	// }
	// //
	// //

	// // encuentra la saida de destino
	// int dst_index = -1;
	// QString dst_layer = "";
	// for (QString out_layer : {"a", "b"})
	// {
		// int out_index = get_output_index(out_layer, event->pos());
		// if (out_index >= 0)
		// {
			// dst_index = out_index;
			// dst_layer = out_layer;
			// break;
		// }
	// }
	// //
	// //

	// connect_channel(src_layer, src_index, dst_layer, dst_index);

	// dragging = false;

	update();
}

void shuffle_gui::mouseMoveEvent(QMouseEvent *event)
{
	mouse_position = event->pos();

	// // si la salida no esta connectada, quita el rellono del circulo
	// // a todas las salidas.
	// for (QString layer : {"a", "b"})
		// for (out_connector &conn : outputs[layer])
			// if (!conn.connected)
				// conn.fill = false;
	// //

	// // si el output existe rellena el circulo
	// if (dragging)
	// {
		// for (QString layer : {"a", "b"})
		// {
			// int out_index = get_output_index(layer, event->pos());
			// if (out_index >= 0 && out_index <= 3)
				// outputs[layer][out_index].fill = true;
		// }
	// }
	// //

	update();
}
