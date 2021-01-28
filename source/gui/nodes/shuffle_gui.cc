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

	output_a = create_output("a");
	output_b = create_output("b");
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

	restore_connections();
}

shuffle_gui::~shuffle_gui(){}

void shuffle_gui::restore_connections()
{
	// ! estos datos tienen que venir del proyecto
	QList <int> in_a = {0, 2, 1, -1};
	//

	int red_input = in_a[0];
	int green_input = in_a[1];
	int blue_input = in_a[2];
	int alpha_input = in_a[3];

	connect_channel("a", 0, red_input);
	connect_channel("a", 1, green_input);
	connect_channel("a", 2, blue_input);
	connect_channel("a", 3, alpha_input);
}

void shuffle_gui::init_connectors()
{
	QColor red = {200, 50, 50};
	QColor green = {50, 200, 50};
	QColor blue = {30, 70, 255};
	QColor white = {200, 200, 200};

	QList <in_connector> in_a;
	in_a.push_back({false, false, {0, 0}, red, -2});
	in_a.push_back({false, false, {0, 0}, green, -2});
	in_a.push_back({false, false, {0, 0}, blue, -2});
	in_a.push_back({false, false, {0, 0}, white, -2});
	inputs.insert("a", in_a);

	QList <in_connector> in_b;
	in_b.push_back({false, false, {0, 0}, red, -2});
	in_b.push_back({false, false, {0, 0}, green, -2});
	in_b.push_back({false, false, {0, 0}, blue, -2});
	in_b.push_back({false, false, {0, 0}, white, -2});
	inputs.insert("b", in_b);

	QList <out_connector> out_a;
	out_a.push_back({false, {0, 0}, red, -1, false, nullptr, nullptr});
	out_a.push_back({false, {0, 0}, green, -1, false, nullptr, nullptr});
	out_a.push_back({false, {0, 0}, blue, -1, false, nullptr, nullptr});
	out_a.push_back({false, {0, 0}, white, -1, false, nullptr, nullptr});
	outputs.insert("a", out_a);

	QList <out_connector> out_b;
	out_b.push_back({false, {0, 0}, red, -1, false, nullptr, nullptr});
	out_b.push_back({false, {0, 0}, green, -1, false, nullptr, nullptr});
	out_b.push_back({false, {0, 0}, blue, -1, false, nullptr, nullptr});
	out_b.push_back({false, {0, 0}, white, -1, false, nullptr, nullptr});
	outputs.insert("b", out_b);
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


QWidget *shuffle_gui::create_output(QString letter)
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

	auto create_output_channel = [=](QString label, int index)
	{
		QWidget *widget = new QWidget();
		widget->setObjectName("output");
		QHBoxLayout *layout = new QHBoxLayout(widget);
		layout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		layout->setMargin(0);

		QLabel *channel = new QLabel(label);
		channel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

		QPushButton *black_button = new QPushButton();
		connect(black_button, &QPushButton::clicked, this, [=]() {
			connect_channel(letter, -1, index);
			update();
		});
		black_button->setProperty("connected", false);
		black_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		black_button->setObjectName("cblack");

		QPushButton *white_button = new QPushButton();
		connect(white_button, &QPushButton::clicked, this, [=]() {
			connect_channel(letter, -2, index);
			update();
		});
		white_button->setProperty("connected", false);
		white_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		white_button->setObjectName("cwhite");

		outputs[letter][index].black_button = black_button;
		outputs[letter][index].white_button = white_button;

		layout->addWidget(black_button);
		layout->addWidget(white_button);
		layout->addWidget(channel);

		output_layout->addWidget(widget);
	};

	output_layout->addWidget(layer_box);
	create_output_channel("rgba.red", 0);
	create_output_channel("rgba.green", 1);
	create_output_channel("rgba.blue", 2);
	create_output_channel("rgba.alpha", 3);

	return output;
}

void shuffle_gui::changed(QString param_name)
{
	int x = input_a->x() + input_a->width() + 30;
	int y = input_a->y() + 50;
}

void shuffle_gui::disconnect_channel(QString letter, int in_index)
{
	if (in_index < 0 || in_index > 3)
		return;

	in_connector &input_ch = inputs[letter][in_index];

	if (input_ch.connected)
	{
		// desconecta la canal de destino
		int out_index = input_ch.ch_output;
		if (out_index >= 0 && out_index <= 3)
		{
			out_connector &output_ch = outputs[letter][out_index];
			output_ch.connected = false;
			output_ch.fill = false;
			output_ch.ch_input = -2;
		}
		//

		// desconecta el canal de origen
		input_ch.connected = false;
		input_ch.ch_output = -2;
		//
	}
}

void shuffle_gui::set_bw_button(QString letter, int index, bool black, bool white)
{
	out_connector &output_ch = outputs[letter][index];
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

void shuffle_gui::connect_channel(QString letter, int in_index, int out_index)
{
	in_connector &input_ch = inputs[letter][in_index];
	out_connector &output_ch = outputs[letter][out_index];

	// si la entrada conectada es la misma a la actual, retorna
	if (in_index >= 0 && in_index <= 3)
		if (input_ch.connected)
			if (input_ch.ch_output == out_index)
				return;
	//

	disconnect_channel(letter, in_index);
	if (out_index < 0 || out_index > 3)
		return;

	// si la entrada es -1 o -2 que son blanco y negro,
	// desconecta la entrada y activa el boton correspondiente
	if (in_index == -2)
	{
		disconnect_channel(letter, output_ch.ch_input);
		set_bw_button(letter, out_index, false, true);
		return;
	}
	else if (in_index == -1)
	{
		disconnect_channel(letter, output_ch.ch_input);
		set_bw_button(letter, out_index, true, false);
		return;
	}
	else
	{
		set_bw_button(letter, out_index, false, false);
	}
	//
	//

	input_ch.connected = true;
	input_ch.ch_output = out_index;

	// desconecta la salida actual del input, ya que en el output no puede
	// haber mas de 1 entrada a la vez
	if (output_ch.connected)
		inputs[letter][output_ch.ch_input].ch_output = -2;
	//

	//
	output_ch.connected = true;
	output_ch.fill = true;
	output_ch.ch_input = in_index;
	//
}

void shuffle_gui::draw_bezier(QPainter &painter, QPoint src, QPoint dst)
{
	float distance_x = dst.x() - src.x();
	float mid_distance_x = distance_x / 1.7;

	QPointF middle_src = {src.x() + mid_distance_x, src.y()};
	QPointF middle_dst = {dst.x() - mid_distance_x, dst.y()};

	int divisions = 30;

	QPointF last_point = src;
	for (int i = 1; i <= divisions; i++)
	{
		float value = float(i) / divisions;
		QPointF point = cubic_bezier(
			QPointF(src),
			middle_src,
			middle_dst,
			QPointF(dst),
			value
		);

		painter.drawLine(last_point, point);
		last_point = point;
	}
}


void shuffle_gui::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing);

	int radius = 5;
	int edge = 2;

	auto draw_in_connector = [=](QString letter, QPoint position, QPainter &painter)
	{
		auto &_connectors = inputs[letter];
		int x = position.x();
		int y = position.y();

		for (in_connector &conn : _connectors)
		{
			painter.setPen(QPen(conn.color, edge));

			y += 20;
			conn.position = {x, y};

			if (conn.dragging)
			{
				painter.setBrush(QBrush(conn.color));
				draw_bezier(painter, conn.position, mouse_position);
			}
			else if (conn.ch_output != -2)
			{
				painter.setBrush(QBrush(conn.color));
				QPoint output_position = outputs[letter][conn.ch_output].position;
				draw_bezier(painter, conn.position, output_position);
			}
			else{
				painter.setBrush(Qt::transparent);
			}

			painter.drawEllipse(conn.position, radius, radius);
		}
	};

	auto draw_out_connector = [=](QString letter, QPoint position, QPainter &painter)
	{
		auto &_connectors = outputs[letter];
		int x = position.x();
		int y = position.y();

		for (out_connector &conn : _connectors)
		{
			painter.setPen(QPen(conn.color, edge));

			if (conn.fill)
				painter.setBrush(QBrush(conn.color));
			else
				painter.setBrush(Qt::transparent);

			y += 20;
			conn.position = {x, y};
			painter.drawEllipse(conn.position, radius, radius);
		}
	};

	int in_x = input_a->x() + input_a->width() + 20;
	int ay = input_a->y() + 30;
	int by = input_b->y() + 30;
	draw_in_connector("a", {in_x, ay}, painter);
	draw_in_connector("b", {in_x, by}, painter);

	int out_x = this->width() - 220;
	int out_ay = output_a->y() + 30;
	int out_by = output_b->y() + 30;
	draw_out_connector("a", {out_x, out_ay}, painter);
	draw_out_connector("b", {out_x, out_by}, painter);

	QWidget::paintEvent(event);
}

int shuffle_gui::get_output_index(QString letter, QPoint position)
{
	// obtiene el index del conector de salida a partir de la position
	auto &_outputs = outputs[letter];
	int out_index = -2;
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

int shuffle_gui::get_input_index(QString letter, QPoint position)
{
	auto &_inputs = inputs[letter];
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
	for (QString letter : {"a", "b"})
	{
		int in_index = get_input_index(letter, event->pos());
		int out_index = get_output_index(letter, event->pos());

		if (in_index == 0 || out_index == 0) connect_channel(letter, 0, 0);
		if (in_index == 1 || out_index == 1) connect_channel(letter, 1, 1);
		if (in_index == 2 || out_index == 2) connect_channel(letter, 2, 2);
		if (in_index == 3 || out_index == 3) connect_channel(letter, 3, 3);
	}
}

void shuffle_gui::mousePressEvent(QMouseEvent *event)
{
	mouse_position = event->pos();

	for (QString letter : {"a", "b"})
	{
		int input_index = get_input_index(letter, event->pos());
		if (input_index >= 0 && input_index <= 3)
		{
			inputs[letter][input_index].dragging = true;
			dragging = true;
		}
	}

	update();
}

void shuffle_gui::mouseReleaseEvent(QMouseEvent *event)
{

	// conecta el canal de salida y desabilita el arrastre en todas las entradas
	for (QString letter : {"a", "b"})
	{
		int out_index = get_output_index(letter, event->pos());

		auto &_inputs = inputs[letter];
		for (int i = 0; i < _inputs.count(); i++)
		{
			in_connector &conn = _inputs[i];

			if (conn.dragging)
				connect_channel(letter, i, out_index);

			conn.dragging = false;
		}
	}

	dragging = false;

	update();
}

void shuffle_gui::mouseMoveEvent(QMouseEvent *event)
{
	mouse_position = event->pos();

	// si la salida no esta connectada, quita el rellono del circulo
	// a todas las salidas.
	for (QString letter : {"a", "b"})
		for (out_connector &conn : outputs[letter])
			if (!conn.connected)
				conn.fill = false;
	//

	// si el output existe rellena el circulo
	if (dragging)
	{
		for (QString letter : {"a", "b"})
		{
			int out_index = get_output_index(letter, event->pos());
			if (out_index >= 0 && out_index <= 3)
				outputs[letter][out_index].fill = true;
		}
	}
	//

	update();
}
