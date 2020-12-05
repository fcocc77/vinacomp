#include <viewer.h>

void viewer::setup_ui()
{
    QWidget *info = info_setup_ui();
    QWidget *player = player_setup_ui();
    QWidget *controls = control_setup_ui();
    QWidget *image_correction = image_correction_setup_ui();

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

    // QComboBox *channels = new QComboBox();
    // layout->addWidget(channels);

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

	// Play
	action *play_forward_action = new action("Play Forward", "L", "play_arrow_a");
    play_forward_action->connect_to(this, [this]() { play_forward(); });
    
	action *play_backward_action = new action("Play Backward", "L", "play_back_a");
    play_backward_action->connect_to(this, [this]() { play_backward(); });
	//

	action *first_frame = new action("Go to first frame", "L", "skip_previous_a");
    first_frame->connect_to(this, [this]() { go_to_first_frame(); });
	action *last_frame = new action("Go to last frame", "L", "skip_next_a");
    last_frame->connect_to(this, [this]() { go_to_last_frame(); });

	action *next_frame_action = new action("Next Frame", "L", "next_frame_a");
    next_frame_action->connect_to(this, [this]() { next_frame(); });
	action *previous_frame_action = new action("Previous Frame", "L", "previous_frame_a");
    previous_frame_action->connect_to(this, [this]() { previous_frame(); });

	action *next_key_frame_action = new action("Next key frame", "L", "next_key_frame_a");
    next_key_frame_action->connect_to(this, [this]() { next_key_frame(); });
	action *previous_key_frame_action = new action("Previous key frame", "L", "previous_key_frame_a");
    previous_key_frame_action->connect_to(this, [this]() { previous_key_frame(); });

	action *skip_forward_action = new action("Skip Forward", "L", "skip_forward_a");
    skip_forward_action->connect_to(this, [this]() { skip_forward(); });
	action *skip_backward_action = new action("Skip Backward", "L", "skip_backward_a");
    skip_backward_action->connect_to(this, [this]() { skip_backward(); });

	action *in_action = new action("Input", "L", "");
	action *out_action = new action("Output", "L", "");

    QLineEdit *frame_edit = new QLineEdit(player_tools);
    frame_edit->setMaximumWidth(40);

    QLineEdit *skip_frame_edit = new QLineEdit(player_tools);
    skip_frame_edit->setText("10");
    skip_frame_edit->setMaximumWidth(30);

    QLineEdit *input_frame_edit = new QLineEdit(player_tools);
    input_frame_edit->setMaximumWidth(30);

    QLineEdit *output_frame_edit = new QLineEdit(player_tools);
    output_frame_edit->setMaximumWidth(30);

    player_tools->add_widget(input_frame_edit);
    player_tools->add_stretch();

    player_tools->add_action(in_action);
    player_tools->add_action(first_frame);
    player_tools->add_action(previous_key_frame_action);
    player_tools->add_action(previous_frame_action);
    player_tools->add_action(play_backward_action);

    player_tools->add_widget(frame_edit);

    player_tools->add_action(play_forward_action);
    player_tools->add_action(next_frame_action);
    player_tools->add_action(next_key_frame_action);
    player_tools->add_action(last_frame);
    player_tools->add_action(out_action);

    player_tools->add_action(skip_backward_action);
    player_tools->add_widget(skip_frame_edit);
    player_tools->add_action(skip_forward_action);

    player_tools->add_stretch();
    player_tools->add_widget(output_frame_edit);

    return player_tools;
}
