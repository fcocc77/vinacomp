#include <shuffle_gui.h>

shuffle_gui::shuffle_gui(QVBoxLayout *controls_layout)
{
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

	input_a = create_input();
	input_b = create_input();
	input_layer_layout->addWidget(input_a);
	input_layer_layout->addWidget(input_b);
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

	output_a = create_output();
	output_b = create_output();
	output_layer_layout->addWidget(output_a);
	output_layer_layout->addWidget(output_b);
	//
	//

	connection_viewer = new QWidget();
	connection_viewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	connection_viewer->setObjectName("connection_viewer");

	main_layout->addWidget(input_layer);
	main_layout->addWidget(connection_viewer);
	main_layout->addWidget(output_layer);

	controls_layout->addWidget(this);

	init_connectors();
}

shuffle_gui::~shuffle_gui(){}

void shuffle_gui::init_connectors()
{
	QColor red = {200, 50, 50};
	QColor green = {50, 200, 50};
	QColor blue = {30, 70, 255};
	QColor white = {200, 200, 200};

	inputs_a.push_back({false, false, {0, 0}, red});
	inputs_a.push_back({false, false, {0, 0}, green});
	inputs_a.push_back({false, false, {0, 0}, blue});
	inputs_a.push_back({false, false, {0, 0}, white});

	inputs_b.push_back({false, false, {0, 0}, red});
	inputs_b.push_back({false, false, {0, 0}, green});
	inputs_b.push_back({false, false, {0, 0}, blue});
	inputs_b.push_back({false, false, {0, 0}, white});

	outputs_a.push_back({false, false, {0, 0}, red});
	outputs_a.push_back({false, false, {0, 0}, green});
	outputs_a.push_back({false, false, {0, 0}, blue});
	outputs_a.push_back({false, false, {0, 0}, white});

	outputs_b.push_back({false, false, {0, 0}, red});
	outputs_b.push_back({false, false, {0, 0}, green});
	outputs_b.push_back({false, false, {0, 0}, blue});
	outputs_b.push_back({false, false, {0, 0}, white});
}

QWidget *shuffle_gui::create_input()
{
	QWidget *input = new QWidget();
	input->setMaximumWidth(200);
	input->setMinimumWidth(200);
	input->setObjectName("input_channels");
	QVBoxLayout *input_layout = new QVBoxLayout(input);

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

	auto create_input_channel = [=](QString label)
	{
		QWidget *widget = new QWidget();
		widget->setObjectName("input");
		QHBoxLayout *layout = new QHBoxLayout(widget);
		layout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		layout->setMargin(0);

		QLabel *channel_label = new QLabel(label);
		channel_label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		layout->addWidget(channel_label);

		input_layout->addWidget(widget);
	};

	input_layout->addWidget(menus);
	create_input_channel("rgba.red");
	create_input_channel("rgba.green");
	create_input_channel("rgba.blue");
	create_input_channel("rgba.alpha");

	return input;
}


QWidget *shuffle_gui::create_output()
{
	QWidget *output = new QWidget();
	output->setMaximumWidth(200);
	output->setMinimumWidth(200);
	output->setObjectName("output_channels");
	QVBoxLayout *output_layout = new QVBoxLayout(output);

	// ! hay que poner las entradas disponible que tiene la imagen
	combo_box *layer_box = new combo_box({
		{"rgba", "rgba"},
		{"depth", "depth"}
	});
	layer_box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	auto create_output_channel = [=](QString label)
	{
		QWidget *widget = new QWidget();
		widget->setObjectName("output");
		QHBoxLayout *layout = new QHBoxLayout(widget);
		layout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		layout->setMargin(0);

		QLabel *channel = new QLabel(label);
		channel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

		QPushButton *black_button = new QPushButton();
		black_button->setProperty("connected", false);
		black_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		black_button->setObjectName("cblack");

		QPushButton *white_button = new QPushButton();
		white_button->setProperty("connected", false);
		white_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		white_button->setObjectName("cwhite");

		layout->addWidget(black_button);
		layout->addWidget(white_button);
		layout->addWidget(channel);

		output_layout->addWidget(widget);
	};

	output_layout->addWidget(layer_box);
	create_output_channel("rgba.red");
	create_output_channel("rgba.green");
	create_output_channel("rgba.blue");
	create_output_channel("rgba.alpha");

	return output;
}

void shuffle_gui::changed(QString param_name)
{
	int x = input_a->x() + input_a->width() + 30;
	int y = input_a->y() + 50;
}

void shuffle_gui::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	int radius = 5;
	int edge = 2;

	auto draw_connectors = [=](QList<connectors> &_connectors, QPoint position, QPainter &painter)
	{
		int x = position.x();
		int y = position.y();

		for (connectors &conn : _connectors)
		{
			painter.setPen(QPen(conn.color, edge));

			if (conn.connected)
				painter.setBrush(QBrush(conn.color));
			else
				painter.setBrush(Qt::transparent);

			y += 20;
			conn.position = {x, y};

			if (conn.dragging)
				painter.drawLine(conn.position, mouse_position);

			painter.drawEllipse(conn.position, radius, radius);
		}
	};

	int in_x = input_a->x() + input_a->width() + 20;
	int ay = input_a->y() + 30;
	int by = input_b->y() + 30;
	draw_connectors(inputs_a, {in_x, ay}, painter);
	draw_connectors(inputs_b, {in_x, by}, painter);

	int out_x = this->width() - 220;
	int out_ay = output_a->y() + 30;
	int out_by = output_b->y() + 30;
	draw_connectors(outputs_a, {out_x, out_ay}, painter);
	draw_connectors(outputs_b, {out_x, out_by}, painter);

	QWidget::paintEvent(event);
}

void shuffle_gui::mousePressEvent(QMouseEvent *event)
{
	mouse_position = event->pos();

	for (connectors &conn : inputs_a)
	{
		int x = conn.position.x();
		int y = conn.position.y();
		float distance = qt::distance_points({x, y}, {event->x(), event->y()});
		if (distance < 10)
		{
			conn.connected = true;
			conn.dragging = true;
			break;
		}
	}

	update();
}

void shuffle_gui::mouseReleaseEvent(QMouseEvent *event) 
{
	// desabilita el arrastre en todas las entradas
	for (connectors &conn : inputs_a)
		conn.dragging = false;

	update();
}

void shuffle_gui::mouseMoveEvent(QMouseEvent *event)
{
	mouse_position = event->pos();
	update();
}
