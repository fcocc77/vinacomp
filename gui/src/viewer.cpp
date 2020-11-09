#include <viewer.hpp>

viewer::viewer(/* args */)
{

    viewer_gl *_viewer_gl = new viewer_gl();
    _viewer_gl->setObjectName("viewer_graphics");

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);
    this->setObjectName("viewer");

    QWidget *info = info_setup_ui();
    QWidget *player = player_setup_ui();
    QWidget *timeline = timeline_setup_ui();
    QWidget *controls = control_setup_ui();
    QWidget *image_correction = image_correction_setup_ui();

    layout->addWidget(controls);
    layout->addWidget(image_correction);
    layout->addWidget(_viewer_gl);
    layout->addWidget(info);
    layout->addWidget(timeline);
    layout->addWidget(player);
}

viewer::~viewer()
{
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
    display_channel->addItem("RGB");
    display_channel->addItem("Red");
    display_channel->addItem("Green");
    display_channel->addItem("Blue");
    display_channel->addItem("Alpha");
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

QWidget *viewer::timeline_setup_ui()
{
    QWidget *widget = new QWidget();
    widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    widget->setObjectName("timeline");
    QHBoxLayout *layout = new QHBoxLayout();
    widget->setLayout(layout);
    widget->setMinimumHeight(50);
    //

    return widget;
}

QWidget *viewer::player_setup_ui()
{
    QWidget *widget = new QWidget();
    widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    widget->setObjectName("player");
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setSpacing(2);
    layout->setMargin(5);
    widget->setLayout(layout);
    //

    int icon_size = 25;

    QPushButton *play_forward = new QPushButton();
    QPushButton *play_backward = new QPushButton();
    qt::set_icon(play_forward, "play_arrow_a", icon_size);
    qt::set_icon(play_backward, "play_back_a", icon_size);

    QPushButton *first_frame = new QPushButton();
    QPushButton *last_frame = new QPushButton();
    qt::set_icon(first_frame, "skip_previous_a", icon_size);
    qt::set_icon(last_frame, "skip_next_a", icon_size);

    QPushButton *next_frame = new QPushButton();
    QPushButton *previous_frame = new QPushButton();
    qt::set_icon(next_frame, "next_frame_a", icon_size);
    qt::set_icon(previous_frame, "previous_frame_a", icon_size);

    QPushButton *next_key_frame = new QPushButton();
    QPushButton *previous_key_frame = new QPushButton();
    qt::set_icon(next_key_frame, "next_key_frame_a", icon_size);
    qt::set_icon(previous_key_frame, "previous_key_frame_a", icon_size);

    QPushButton *skip_forward = new QPushButton();
    QPushButton *skip_backward = new QPushButton();
    qt::set_icon(skip_forward, "skip_forward_a", icon_size);
    qt::set_icon(skip_backward, "skip_backward_a", icon_size);

    QPushButton *in = new QPushButton("I");
    QPushButton *out = new QPushButton("O");

    QLineEdit *frame_edit = new QLineEdit();
    frame_edit->setMaximumWidth(40);

    QLineEdit *skip_frame_edit = new QLineEdit();
    skip_frame_edit->setText("10");
    skip_frame_edit->setMaximumWidth(30);

    QLineEdit *input_frame_edit = new QLineEdit();
    input_frame_edit->setMaximumWidth(30);

    QLineEdit *output_frame_edit = new QLineEdit();
    output_frame_edit->setMaximumWidth(30);

    layout->addWidget(input_frame_edit);
    layout->addStretch();

    layout->addWidget(in);
    layout->addWidget(first_frame);
    layout->addWidget(previous_key_frame);
    layout->addWidget(previous_frame);
    layout->addWidget(play_backward);

    layout->addWidget(frame_edit);

    layout->addWidget(play_forward);
    layout->addWidget(next_frame);
    layout->addWidget(next_key_frame);
    layout->addWidget(last_frame);
    layout->addWidget(out);

    QSpacerItem *spacer = new QSpacerItem(40, 20);
    layout->addItem(spacer);

    layout->addWidget(skip_backward);
    layout->addWidget(skip_frame_edit);
    layout->addWidget(skip_forward);

    layout->addStretch();
    layout->addWidget(output_frame_edit);

    return widget;
}