#include <viewer.h>

void viewer::setup_ui()
{
    QWidget *info = info_setup_ui();
    QWidget *player = player_setup_ui();
    QWidget *controls = control_setup_ui();
    QWidget *image_correction = image_correction_setup_ui();

	_time_line = new time_line(this, frame_edit, input_frame_edit, output_frame_edit, skip_frame_edit);

    layout->addWidget(controls);
    layout->addWidget(image_correction);
    layout->addWidget(_viewer_gl);
    layout->addWidget(info);
    layout->addWidget(_time_line);
    layout->addWidget(player);
}

QWidget *viewer::control_setup_ui()
{
    QWidget *widget = new QWidget();
    widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    widget->setObjectName("controls");
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setSpacing(2);
    layout->setMargin(5);
    widget->setLayout(layout);
    //

    int icon_size = 25;

    combo_box *layers = new combo_box();
    layout->addWidget(layers);

    combo_box *display_channel = new combo_box();
    display_channel->add_item("RGB");
    display_channel->add_item("Red");
    display_channel->add_item("Green");
    display_channel->add_item("Blue");
    display_channel->add_item("Alpha");
    layout->addWidget(display_channel);

    layout->addStretch();

    QPushButton *free_ram = new QPushButton();
    qt::set_icon(free_ram, "free_ram_a", icon_size);
    layout->addWidget(free_ram);

    QPushButton *out_frame = new QPushButton();
    qt::set_icon(out_frame, "out_frame_a", icon_size);
    layout->addWidget(out_frame);

    QPushButton *render_area = new QPushButton();
    qt::set_icon(render_area, "render_area_a", icon_size);
    layout->addWidget(render_area);

    QPushButton *proxy = new QPushButton();
    qt::set_icon(proxy, "proxy_a", icon_size);
    layout->addWidget(proxy);

    QPushButton *multi_lines = new QPushButton();
    qt::set_icon(multi_lines, "multi_lines_a", icon_size);
    layout->addWidget(multi_lines);

    QPushButton *refresh = new QPushButton();
    qt::set_icon(refresh, "refresh_a", icon_size);
    layout->addWidget(refresh);

    QPushButton *pause = new QPushButton();
    qt::set_icon(pause, "pause_a", icon_size);
    layout->addWidget(pause);

    return widget;
}

QWidget *viewer::image_correction_setup_ui()
{
    QWidget *widget = new QWidget();
    widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    widget->setObjectName("image_correction");
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setSpacing(2);
    layout->setMargin(5);
    widget->setLayout(layout);
    //

    int icon_size = 25;

    QPushButton *gain = new QPushButton();
    qt::set_icon(gain, "gain_a", icon_size);
    layout->addWidget(gain);

    QLineEdit *gain_edit = new QLineEdit();
    gain_edit->setMaximumWidth(50);
    layout->addWidget(gain_edit);

    slider *gain_slider = new slider();
    layout->addWidget(gain_slider);

    QPushButton *gamma = new QPushButton();
    qt::set_icon(gamma, "gamma_a", icon_size);
    layout->addWidget(gamma);

    QLineEdit *gamma_edit = new QLineEdit();
    gamma_edit->setMaximumWidth(50);
    layout->addWidget(gamma_edit);

    slider *gamma_slider = new slider();
    layout->addWidget(gamma_slider);

    layout->addStretch();

    return widget;
}

QWidget *viewer::info_setup_ui()
{
    QWidget *widget = new QWidget();
    widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    widget->setObjectName("info");
    QHBoxLayout *layout = new QHBoxLayout();
    widget->setLayout(layout);
    widget->setMinimumHeight(10);
    //

    return widget;
}

QWidget *viewer::player_setup_ui()
{
	tools *player_tools = new tools();
	player_tools->get_layout()->setSpacing(7);
	player_tools->setObjectName("player");

	play_forward_action = new action("Play Forward", "", "play_arrow");
    play_forward_action->connect_to(this, [this]() { play(QTimeLine::Forward); });

	stop_forward_action = new action("Stop", "", "stop");
    stop_forward_action->connect_to(this, [this]() { stop(); });
	stop_forward_action->set_visible(false);

	play_backward_action = new action("Play Backward", "", "play_back");
    play_backward_action->connect_to(this, [this]() { play(QTimeLine::Backward); });

	stop_backward_action = new action("Stop", "", "stop");
    stop_backward_action->connect_to(this, [this]() { stop(); });
	stop_backward_action->set_visible(false);

	action *first_frame = new action("Go to first frame", "", "skip_previous");
    first_frame->connect_to(this, [this]() { go_to_first_frame(); });
	action *last_frame = new action("Go to last frame", "", "skip_next");
    last_frame->connect_to(this, [this]() { go_to_last_frame(); });

	action *next_frame_action = new action("Next Frame", "", "next_frame");
    next_frame_action->connect_to(this, [this]() { next_frame(); });
	action *previous_frame_action = new action("Previous Frame", "", "previous_frame");
    previous_frame_action->connect_to(this, [this]() { previous_frame(); });

	action *next_key_frame_action = new action("Next key frame", "", "next_key_frame");
    next_key_frame_action->connect_to(this, [this]() { next_key_frame(); });
	action *previous_key_frame_action = new action("Previous key frame", "", "previous_key_frame");
    previous_key_frame_action->connect_to(this, [this]() { previous_key_frame(); });

	action *skip_forward_action = new action("Skip Forward", "", "skip_forward");
    skip_forward_action->connect_to(this, [this]() { skip_forward(); });
	action *skip_backward_action = new action("Skip Backward", "", "skip_backward");
    skip_backward_action->connect_to(this, [this]() { skip_backward(); });

	action *in_action = new action("Input", "", "input");
	action *out_action = new action("Output", "", "output");

    frame_edit = new QLineEdit(player_tools);
	frame_edit->setValidator( new QIntValidator(-100000, 100000, this) );  // Solo numeros
    frame_edit->setMaximumWidth(40);

    skip_frame_edit = new QLineEdit(player_tools);
	skip_frame_edit->setValidator( new QIntValidator(1, 100, this) ); // Solo numeros del 1 al 100
    skip_frame_edit->setText("10");
    skip_frame_edit->setMaximumWidth(30);

    input_frame_edit = new QLineEdit(player_tools);
	input_frame_edit->setValidator( new QIntValidator(-100000, 100000, this) );  // Solo numeros
    input_frame_edit->setMaximumWidth(30);

    output_frame_edit = new QLineEdit(player_tools);
	output_frame_edit->setValidator( new QIntValidator(-100000, 100000, this) );  // Solo numeros
    output_frame_edit->setMaximumWidth(30);

	frame_rate_edit = new QLineEdit(player_tools);
	frame_rate_edit->setMaximumWidth(40);
	frame_rate_edit->setObjectName("frame_rate_edit");
	connect(frame_rate_edit, &QLineEdit::editingFinished, this, [=](){
		set_frame_rate(frame_rate_edit->text().toInt());
	});

	frame_rate_menu = new combo_box();
	frame_rate_menu->add_item("12");
	frame_rate_menu->add_item("24");
	frame_rate_menu->add_item("30");
	frame_rate_menu->add_item("48");
	frame_rate_menu->add_item("50");
	frame_rate_menu->add_item("60");

	combo_box *play_back_options = new combo_box();
	connect(play_back_options, &combo_box::changed, this, [](QString name, int index){
		print(name);
	});
	play_back_options->add_item("Repeat");
	play_back_options->add_item("Bounce");
	play_back_options->add_item("Stop");
	play_back_options->add_item("Continue");

    player_tools->add_widget(input_frame_edit);
    player_tools->add_action(in_action);
    player_tools->add_widget(frame_rate_edit);
    player_tools->add_widget(frame_rate_menu);
    player_tools->add_stretch();

    player_tools->add_widget(play_back_options);
    player_tools->add_action(first_frame);
    player_tools->add_action(previous_key_frame_action);
    player_tools->add_action(previous_frame_action);
    player_tools->add_action(stop_backward_action);
    player_tools->add_action(play_backward_action);

    player_tools->add_widget(frame_edit);

    player_tools->add_action(play_forward_action);
    player_tools->add_action(stop_forward_action);
    player_tools->add_action(next_frame_action);
    player_tools->add_action(next_key_frame_action);
    player_tools->add_action(last_frame);

	player_tools->add_separator();

    player_tools->add_action(skip_backward_action);
    player_tools->add_widget(skip_frame_edit);
    player_tools->add_action(skip_forward_action);

    player_tools->add_stretch();
    player_tools->add_action(out_action);
    player_tools->add_widget(output_frame_edit);

    return player_tools;
}
