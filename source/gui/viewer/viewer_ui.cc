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

	combo_box *display_channel = new combo_box({"RGB", "Red", "Green", "Blue", "Alpha"});
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
	tools *bar = new tools();
	bar->setObjectName("image_correction");

	action *gain_action = new action("Gain", "", "gain");
	gain_action->set_checkable();
	gain_action->connect_to(this, [](){
	});

	knob_floating *gain_knob = new knob_floating(0.015, 64);

	action *gamma_action = new action("Gamma", "", "gamma");
	gamma_action->set_checkable();
	gamma_action->connect_to(this, [](){
	});

	knob_floating *gamma_knob = new knob_floating(0, 4);

	bar->add_action(gain_action);
	bar->add_widget(gain_knob);
	bar->add_action(gamma_action);
	bar->add_widget(gamma_knob);

	return bar;
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

	//
	//
	play_forward_action = new action("Play Forward", "", "play_arrow");
    play_forward_action->connect_to(this, [this]() { play(QTimeLine::Forward); });
	//
	//

	//
	//
	stop_forward_action = new action("Stop", "", "stop");
    stop_forward_action->connect_to(this, [this]() { stop(); });
	stop_forward_action->set_visible(false);
	//
	//

	//
	//
	play_backward_action = new action("Play Backward", "", "play_back");
    play_backward_action->connect_to(this, [this]() { play(QTimeLine::Backward); });
	//
	//

	//
	//
	stop_backward_action = new action("Stop", "", "stop");
    stop_backward_action->connect_to(this, [this]() { stop(); });
	stop_backward_action->set_visible(false);
	//
	//

	//
	//
	action *first_frame = new action("Go to first frame", "", "skip_previous");
    first_frame->connect_to(this, [this]() { go_to_first_frame(); });
	action *last_frame = new action("Go to last frame", "", "skip_next");
    last_frame->connect_to(this, [this]() { go_to_last_frame(); });
	//
	//

	//
	//
	action *next_frame_action = new action("Next Frame", "", "next_frame");
    next_frame_action->connect_to(this, [this]() { next_frame(); });
	action *previous_frame_action = new action("Previous Frame", "", "previous_frame");
    previous_frame_action->connect_to(this, [this]() { previous_frame(); });
	//
	//

	//
	//
	action *next_key_frame_action = new action("Next key frame", "", "next_key_frame");
    next_key_frame_action->connect_to(this, [this]() { next_key_frame(); });
	action *previous_key_frame_action = new action("Previous key frame", "", "previous_key_frame");
    previous_key_frame_action->connect_to(this, [this]() { previous_key_frame(); });
	//
	//

	//
	//
	action *skip_forward_action = new action("Skip Forward", "", "skip_forward");
    skip_forward_action->connect_to(this, [this]() { skip_forward(); });
	action *skip_backward_action = new action("Skip Backward", "", "skip_backward");
    skip_backward_action->connect_to(this, [this]() { skip_backward(); });
	//
	//

	//
	//
	input_action = new action("Input", "I", "input");
	input_action->set_object_name("input");
	input_action->set_checkable();
	input_action->connect_to(this, [this]() {
		set_in_out(current_frame, output);
	});
	//
	//

	//
	//
	output_action = new action("Output", "O", "output");
	output_action->set_object_name("output");
	output_action->set_checkable();
	output_action->connect_to(this, [this]() {
		set_in_out(input, current_frame);
	});
	//
	//

	//
	//
	visibility_in_out_action = new action("Visibility Input and Output", "", "in_out");
	visibility_in_out_action->connect_to(this, [=](){
		enable_in_out(!in_out);
	});
	visibility_in_out_action->set_object_name("in_out");
	visibility_in_out_action->set_checkable(true);
	//
	//

	//
	//
    frame_edit = new QLineEdit(player_tools);
	connect(frame_edit, &QLineEdit::editingFinished, this, [=](){
		set_frame(frame_edit->text().toInt());
	});
	frame_edit->setValidator( new QIntValidator(-100000, 100000, this) );  // Solo numeros
    frame_edit->setMaximumWidth(40);
	//
	//

	//
	//
    skip_frame_edit = new QLineEdit(player_tools);
	skip_frame_edit->setValidator( new QIntValidator(1, 100, this) ); // Solo numeros del 1 al 100
    skip_frame_edit->setText("10");
    skip_frame_edit->setMaximumWidth(30);
	//
	//

	//
	//
    input_frame_edit = new QLineEdit(player_tools);
	connect(input_frame_edit, &QLineEdit::editingFinished, this, [=](){
		set_in_out(input_frame_edit->text().toInt(), output);
	});
	input_frame_edit->setValidator( new QIntValidator(-100000, 100000, this) );  // Solo numeros
    input_frame_edit->setMaximumWidth(30);
	//
	//

	//
	//
    output_frame_edit = new QLineEdit(player_tools);
	connect(output_frame_edit, &QLineEdit::editingFinished, this, [=](){
		set_in_out(input, output_frame_edit->text().toInt());
	});
	output_frame_edit->setValidator( new QIntValidator(-100000, 100000, this) );  // Solo numeros
    output_frame_edit->setMaximumWidth(30);
	//
	//

	//
	//
	frame_rate_menu = new combo_box({"12", "24", "30", "48", "50", "60"});
	connect(frame_rate_menu, &combo_box::changed, this, [=](QString name){
		set_frame_rate(name.toInt());
	});
	//
	//

	//
	// Play Back Option
	play_back_options = new combo_box({"Repeat", "Bounce", "Stop"});
	connect(play_back_options, &combo_box::changed, this, [=](QString name){
		set_playing_option(name);
	});
	//
	//

	//
	//
	visible_range = new combo_box({"Global", "Input", "In/Out", "Visible"});
	visible_range->add_item("Global");
	//
	//

    player_tools->add_action(input_action);
    player_tools->add_widget(input_frame_edit);
	player_tools->add_separator();
    player_tools->add_widget(frame_rate_menu);
    player_tools->add_widget(visible_range);
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
    player_tools->add_action(visibility_in_out_action);
	player_tools->add_separator();
    player_tools->add_widget(output_frame_edit);
    player_tools->add_action(output_action);

    return player_tools;
}
