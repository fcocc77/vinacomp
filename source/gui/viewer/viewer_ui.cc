#include <viewer.h>
#include <knob_floating.h>
#include <knob_channels.h>
#include <tools.h>

void viewer::setup_ui()
{
    QWidget *info = info_setup_ui();
    QWidget *player = player_setup_ui();
    QWidget *controls = control_setup_ui();
    QWidget *image_correction = image_correction_setup_ui();

    _time_line =
        new time_line(this, frame_edit, input_frame_edit, output_frame_edit, skip_frame_edit);

    layout->addWidget(controls);
    layout->addWidget(image_correction);
    layout->addWidget(_viewer_gl);
    layout->addWidget(info);
    layout->addWidget(_time_line);
    layout->addWidget(player);

    create_menu();
}

void viewer::create_menu()
{
    menu = new QMenu(_vinacomp);

    action *overlay_action = new action("Overlay", "Q");
    overlay_action->set_checkable();
    overlay_action->set_checked(true);
    overlay_action->connect_to(this,
                               [=]() { _viewer_gl->set_overlay(overlay_action->is_checked()); });

    menu->addAction(overlay_action);

    menu->addSeparator();

    menu->addAction(pause_action);
    menu->addAction(out_frame_action);
    menu->addAction(render_area_action);
    menu->addAction(multi_lines_action);
    menu->addAction(refresh_action);
}

QWidget *viewer::control_setup_ui()
{
    tools *bar = new tools();
    bar->setObjectName("controls");

    // Layers
    knob_channels *layers = new knob_channels(project);
    connect(layers, &knob_channels::changed, this,
            [=](QString layer) { change_layer(layer); });

    bar->add_widget(layers);
    //

    bar->add_separator();

    // Channels
    display_channel = new combo_box({{"RGB", "rgb"},
                                     {"Red", "red"},
                                     {"Green", "green"},
                                     {"Blue", "blue"},
                                     {"Alpha", "alpha"}},
                                    0, this);

    connect(display_channel, &combo_box::changed, this,
            [=](QVariant value, int index) { change_channel(index); });

    display_channel->add_shortcut(1, "R");
    display_channel->add_shortcut(2, "G");
    display_channel->add_shortcut(3, "B");
    display_channel->add_shortcut(4, "A");
    bar->add_widget(display_channel);
    //

    bar->add_stretch();

    action *purge_ram_action = new action("Free Ram", "free_ram", "free_ram");
    purge_ram_action->connect_to(this, [this]() { purge_ram(); });
    bar->add_action(purge_ram_action);

    bar->add_separator();

    handler_snap_action = new action("Handler Snap", "handler_snap", "magnet");
    handler_snap_action->set_checkable();
    handler_snap_action->connect_to(this, [this]() { set_handler_snap(true); });
    bar->add_action(handler_snap_action);

    out_frame_action =
        new action("Show pixels out of frame", "out_frame", "out_frame");
    out_frame_action->set_checkable();
    out_frame_action->connect_to(this, [this]() { set_outside_pixels(true); });
    bar->add_action(out_frame_action);

    render_area_action =
        new action("Render Area", "render_area", "render_area");
    render_area_action->set_checkable();
    render_area_action->connect_to(this, [this]() { set_render_area(true); });
    bar->add_action(render_area_action);

    bar->add_separator();

    proxy_action = new action("Proxy", "proxy", "proxy");
    proxy_action->set_checkable();
    proxy_action->connect_to(this, [this]() { set_proxy(true); });
    bar->add_action(proxy_action);

    multi_lines_action =
        new action("Render all lines", "multi_lines", "multi_lines");
    multi_lines_action->set_checkable();
    multi_lines_action->connect_to(this, [this]() { set_multi_lines(true); });
    bar->add_action(multi_lines_action);

    bar->add_separator();

    refresh_action = new action("Refresh", "refresh", "refresh");
    refresh_action->connect_to(this, []() {});
    bar->add_action(refresh_action);

    pause_action = new action("Pause Render", "pause", "pause");
    pause_action->set_checkable();
    pause_action->connect_to(this, [=]() { render_pause = !render_pause; });
    bar->add_action(pause_action);

    return bar;
}

QWidget *viewer::image_correction_setup_ui()
{
    tools *bar = new tools();
    bar->setObjectName("image_correction");

    action *gain_action = new action("Gain", "", "gain");

    knob_floating *gain_knob = new knob_floating(0.015, 64, 1, false, true);
    connect(gain_knob, &knob_floating::changed, this, [=](float value) {
        gain = value;
        gain_action->set_checked(true);
    });

    gain_action->set_checkable();
    gain_action->connect_to(this, [=]() {
        if (gain_action->is_checked())
            gain_knob->set_value(gain, 0, false);
        else
            gain_knob->set_value(1, 0, false);
    });

    action *gamma_action = new action("Gamma", "", "gamma");
    knob_floating *gamma_knob = new knob_floating(0, 4, 1, false, true);
    connect(gamma_knob, &knob_floating::changed, this, [=](float value) {
        gamma = value;
        gamma_action->set_checked(true);
    });

    gamma_action->set_checkable();
    gamma_action->connect_to(this, [=]() {
        if (gamma_action->is_checked())
            gamma_knob->set_value(gamma, 0, false);
        else
            gamma_knob->set_value(1, 0, false);
    });

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
    play_forward_action->connect_to(this, [this]() { play(vina::right); });
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
    play_backward_action->connect_to(this, [this]() { play(vina::left); });
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
    input_action->connect_to(this, [this]() { set_in_out(project->frame, output); });
    //
    //

    //
    //
    output_action = new action("Output", "O", "output");
    output_action->set_object_name("output");
    output_action->set_checkable();
    output_action->connect_to(this, [this]() { set_in_out(input, project->frame); });
    //
    //

    //
    //
    visibility_in_out_action = new action("Visibility Input and Output", "", "in_out");
    visibility_in_out_action->connect_to(this, [=]() { enable_in_out(!in_out); });
    visibility_in_out_action->set_object_name("in_out");
    visibility_in_out_action->set_checkable(true);
    //
    //

    //
    //
    frame_edit = new QLineEdit(player_tools);
    connect(frame_edit, &QLineEdit::editingFinished, this,
            [=]() { set_frame(frame_edit->text().toInt()); });
    frame_edit->setValidator(new QIntValidator(-100000, 100000, this)); // Solo numeros
    frame_edit->setMaximumWidth(40);
    //
    //

    //
    //
    skip_frame_edit = new QLineEdit(player_tools);
    skip_frame_edit->setValidator(new QIntValidator(1, 100, this)); // Solo numeros del 1 al 100
    skip_frame_edit->setText("10");
    skip_frame_edit->setMaximumWidth(30);
    //
    //

    //
    //
    input_frame_edit = new QLineEdit(player_tools);
    connect(input_frame_edit, &QLineEdit::editingFinished, this,
            [=]() { set_in_out(input_frame_edit->text().toInt(), output); });
    input_frame_edit->setValidator(new QIntValidator(-100000, 100000, this)); // Solo numeros
    input_frame_edit->setMaximumWidth(30);
    //
    //

    //
    //
    output_frame_edit = new QLineEdit(player_tools);
    connect(output_frame_edit, &QLineEdit::editingFinished, this,
            [=]() { set_in_out(input, output_frame_edit->text().toInt()); });
    output_frame_edit->setValidator(new QIntValidator(-100000, 100000, this)); // Solo numeros
    output_frame_edit->setMaximumWidth(30);
    //
    //

    //
    //
    frame_rate_menu = new combo_box({{"12 fps", 12},
                                     {"24 fps", 24},
                                     {"30 fps", 30},
                                     {"48 fps", 48},
                                     {"50 fps", 50},
                                     {"60 fps", 60}});
    frame_rate_menu->setObjectName("frame_rate_menu");
    connect(frame_rate_menu, &combo_box::changed, this,
            [=](QVariant value) { set_frame_rate(value.toInt()); });
    //
    //

    //
    // Play Back Option
    play_back_options =
        new combo_box({{"Repeat", "repeat"}, {"Bounce", "bounce"}, {"Stop", "stop"}});
    connect(play_back_options, &combo_box::changed, this,
            [=](QVariant value) { set_playing_option(value.toString()); });
    //
    //

    //
    //
    range_way_menu = new combo_box({{"Global", "global"}, {"Input", "input"}, {"In/Out", "inout"}});
    connect(range_way_menu, &combo_box::changed, this, [=](QVariant value) {
        input_range_way = value.toString();
        update_input_range();
    });
    //
    //

    player_tools->add_action(input_action);
    player_tools->add_widget(input_frame_edit);
    player_tools->add_separator();
    player_tools->add_widget(frame_rate_menu);
    player_tools->add_widget(range_way_menu);
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
