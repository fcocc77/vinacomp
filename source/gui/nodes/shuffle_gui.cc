#include <shuffle_gui.h>

shuffle_gui::shuffle_gui(QVBoxLayout *controls_layout)
{
	this->setObjectName("shuffle_node");
	QHBoxLayout *main_layout = new QHBoxLayout(this);

	QWidget *input_layer = new QWidget();
	input_layer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	QVBoxLayout *input_layer_layout = new QVBoxLayout(input_layer);
	QLabel *input_label = new QLabel("Input Layer");
	input_layer_layout->addWidget(input_label);
	input_layer_layout->addWidget(create_input());
	input_layer_layout->addWidget(create_input());

	QWidget *output_layer = new QWidget();
	output_layer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	QVBoxLayout *output_layer_layout = new QVBoxLayout(output_layer);
	QLabel *output_label = new QLabel("Output Layer");
	output_layer_layout->addWidget(output_label);
	output_layer_layout->addWidget(create_output());
	output_layer_layout->addWidget(create_output());


	QWidget *connection_viewer = new QWidget();
	connection_viewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	connection_viewer->setObjectName("connection_viewer");

	main_layout->addWidget(input_layer);
	main_layout->addWidget(connection_viewer);
	main_layout->addWidget(output_layer);

	controls_layout->addWidget(this);
}

shuffle_gui::~shuffle_gui(){}

void shuffle_gui::create_input_channel(QVBoxLayout *parent_layout, QString label, QString color_name)
{
	QWidget *widget = new QWidget();
	widget->setObjectName("input");
	QHBoxLayout *layout = new QHBoxLayout(widget);
	layout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	layout->setMargin(0);

	QLabel *channel = new QLabel(label);
	channel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	QPushButton *channel_connector = new QPushButton();
	channel_connector->setProperty("connected", false);
	channel_connector->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	channel_connector->setObjectName("c" + color_name);

	if (color_name == "green")
		channel_connector->setProperty("connected", true);

	layout->addWidget(channel);
	layout->addWidget(channel_connector);

	parent_layout->addWidget(widget);
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

	input_layout->addWidget(menus);
	create_input_channel(input_layout, "rgba.red", "red");
	create_input_channel(input_layout, "rgba.green", "green");
	create_input_channel(input_layout, "rgba.blue", "blue");
	create_input_channel(input_layout, "rgba.alpha", "alpha");

	return input;
}

void shuffle_gui::create_output_channel(QVBoxLayout *parent_layout, QString label, QString color_name)
{
	QWidget *widget = new QWidget();
	widget->setObjectName("output");
	QHBoxLayout *layout = new QHBoxLayout(widget);
	layout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	layout->setMargin(0);

	QLabel *channel = new QLabel(label);
	channel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	QPushButton *rgb_connector = new QPushButton();
	rgb_connector->setProperty("connected", false);
	rgb_connector->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	rgb_connector->setObjectName("c" + color_name);

	QPushButton *black_button = new QPushButton();
	black_button->setProperty("connected", false);
	black_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	black_button->setObjectName("cblack");

	QPushButton *white_button = new QPushButton();
	white_button->setProperty("connected", false);
	white_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	white_button->setObjectName("cwhite");

	layout->addWidget(rgb_connector);
	layout->addWidget(black_button);
	layout->addWidget(white_button);
	layout->addWidget(channel);

	parent_layout->addWidget(widget);
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

	output_layout->addWidget(layer_box);
	create_output_channel(output_layout, "rgba.red", "red");
	create_output_channel(output_layout, "rgba.green", "green");
	create_output_channel(output_layout, "rgba.blue", "blue");
	create_output_channel(output_layout, "rgba.alpha", "alpha");

	return output;
}

void shuffle_gui::changed(QString param_name)
{
}

void shuffle_gui::paintEvent(QPaintEvent *event)
{
	// QPainter painter;
    // painter.begin(this);
    // painter.fillRect(event->rect(), Qt::white);
    // QWidget::paintEvent(event);
    // painter.end();

	// QPainter painter(this);
	// print("painter");
}
