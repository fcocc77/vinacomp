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

	out_layer_a = new out_layer(this, "a");
	out_layer_b = new out_layer(this, "b");
	output_layer_layout->addWidget(out_layer_a);
	output_layer_layout->addWidget(out_layer_b);

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

	auto in_conns = in_layer_a->get_connectors();
	auto out_conns = out_layer_a->get_connectors();

	for (int i = 0; i < 4; i++)
	{
		int index = in_a[i];

		if (index == -1)
			out_conns[i]->set_bw_button(true, false);
		else if (index == -2)
			out_conns[i]->set_bw_button(false, true);
		else 
			in_conns[index]->connect_output(out_conns[i]);
	}
}

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

	update();
}

void shuffle_gui::mouseMoveEvent(QMouseEvent *event)
{
	mouse_position = event->pos();
	update();
}

void shuffle_gui::mouseDoubleClickEvent(QMouseEvent *event)
{
	in_connector *in_conn = get_in_connector(event->pos());
	out_connector *out_conn = get_out_connector(event->pos());

	if (in_conn)
	{
		QString layer = in_conn->get_layer();
		int index = in_conn->get_index();
		out_connector *_out_conn;
		if (layer == "a")
			_out_conn = out_layer_a->get_connectors()[index];
		else
			_out_conn = out_layer_b->get_connectors()[index];

		in_conn->connect_output(_out_conn);
	}

	if (out_conn)
	{
		QString layer = out_conn->get_layer();
		int index = out_conn->get_index();

		in_connector *_in_conn;
		if (layer == "a")
			_in_conn = in_layer_a->get_connectors()[index];
		else
			_in_conn = in_layer_b->get_connectors()[index];

		_in_conn->connect_output(out_conn);
	}
}
