#include <knob_color.h>

void knob_color::setup_ui()
{
    top_layout->setMargin(0);
    content_layout->setMargin(0);
    main_layout->setMargin(0);

    content_layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    mono_edit->setMaximumWidth(50);
    mono_edit->setText(QString::number(red));

    // Caja de colores separados
    separate_colors_box_layout->setMargin(0);

    red_hedit->setObjectName("red_edit");
    green_hedit->setObjectName("green_edit");
    blue_hedit->setObjectName("blue_edit");
    alpha_hedit->setObjectName("alpha_edit");

    separate_colors_box->hide();

    // Caja de sliders de rgb
    separate_colors_slider_layout->setMargin(0);
    separate_colors_slider_layout->setSpacing(5);

    // Red
    red_layout->setMargin(0);
    red_vedit->setMaximumWidth(50);
    red_vedit->setObjectName("red_edit");
    red_slider->setObjectName("red_slider");
    //

    // Green
    green_layout->setMargin(0);
    green_vedit->setMaximumWidth(50);
    green_vedit->setObjectName("green_edit");
    green_slider->setObjectName("green_slider");
    //

    // Blue
    blue_layout->setMargin(0);
    blue_vedit->setMaximumWidth(50);
    blue_vedit->setObjectName("blue_edit");
    blue_slider->setObjectName("blue_slider");
    //

    // Alpha
    alpha_layout->setMargin(0);
    alpha_vedit->setMaximumWidth(50);
    alpha_vedit->setObjectName("alpha_edit");
    alpha_slider->setObjectName("alpha_slider");
    //
    separate_colors_slider->hide();

    //
    //

    // Opciones de botones
    color_sample_button->setObjectName("small_button");
    color_sample_button->setAutoFillBackground(true);
    color_sample_button->setSizePolicy(QSizePolicy::Fixed,
                                       QSizePolicy::Expanding);

    palette_button->setObjectName("small_button");
    palette_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    qt::set_icon(palette_button, "color_normal", icon_size);

    mono_color_button->setObjectName("small_button");
    mono_color_button->setSizePolicy(QSizePolicy::Fixed,
                                     QSizePolicy::Expanding);
    mono_color_button->setText("4");

    hue_button->setObjectName("small_button");
    hue_button->setText("HUE");

    rgb_button->setObjectName("small_button");
    rgb_button->setText("RGB");

    hsl_button->setObjectName("small_button");
    hsl_button->setText("HSL");

    picker_button->setObjectName("small_button");
    picker_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    qt::set_icon(picker_button, "picker_normal", icon_size);

    // Layout
    separate_colors_box_layout->addWidget(red_hedit);
    separate_colors_box_layout->addWidget(green_hedit);
    separate_colors_box_layout->addWidget(blue_hedit);
    separate_colors_box_layout->addWidget(alpha_hedit);

    red_layout->addWidget(red_vedit);
    red_layout->addWidget(red_slider);

    green_layout->addWidget(green_vedit);
    green_layout->addWidget(green_slider);

    blue_layout->addWidget(blue_vedit);
    blue_layout->addWidget(blue_slider);

    alpha_layout->addWidget(alpha_vedit);
    alpha_layout->addWidget(alpha_slider);

    separate_colors_slider_layout->addWidget(red_widget);
    separate_colors_slider_layout->addWidget(green_widget);
    separate_colors_slider_layout->addWidget(blue_widget);
    separate_colors_slider_layout->addWidget(alpha_widget);

    top_layout->addWidget(mono_edit);
    top_layout->addWidget(mono_slider);
    top_layout->addWidget(separate_colors_box);
    top_layout->addWidget(color_sample_button);
    top_layout->addWidget(picker_button);
    top_layout->addWidget(hue_button);
    top_layout->addWidget(rgb_button);
    top_layout->addWidget(hsl_button);
    top_layout->addWidget(palette_button);
    top_layout->addWidget(mono_color_button);

    content_layout->addWidget(top_widget);
    content_layout->addWidget(separate_colors_slider);

    main_layout->addWidget(init_space);
    main_layout->addWidget(content_widget);
}

void knob_color::connections()
{
    // Mono Color
    connect(mono_slider, &slider::moved, this,
            [=](float value) { set_color(value, value, value, value, false); });
    connect(mono_edit, &QLineEdit::editingFinished, this, [=]() {
        float value = mono_edit->text().toDouble();
        set_color(value, value, value, value);
    });
    //
    //

    // Sliders
    connect(red_slider, &slider::moved, this,
            [=](float value) { set_color(value, green, blue, alpha, false); });
    connect(green_slider, &slider::moved, this,
            [=](float value) { set_color(red, value, blue, alpha, false); });
    connect(blue_slider, &slider::moved, this,
            [=](float value) { set_color(red, green, value, alpha, false); });
    connect(alpha_slider, &slider::moved, this,
            [=](float value) { set_color(red, green, blue, value, false); });
    //
    //

    // Edits Verticales
    connect(red_vedit, &QLineEdit::editingFinished, this, [=]() {
        float value = red_vedit->text().toDouble();
        set_color(value, green, blue, alpha);
    });
    connect(green_vedit, &QLineEdit::editingFinished, this, [=]() {
        float value = green_vedit->text().toDouble();
        set_color(red, value, blue, alpha);
    });
    connect(blue_vedit, &QLineEdit::editingFinished, this, [=]() {
        float value = blue_vedit->text().toDouble();
        set_color(red, green, value, alpha);
    });
    connect(alpha_vedit, &QLineEdit::editingFinished, this, [=]() {
        float value = alpha_vedit->text().toDouble();
        set_color(red, green, blue, value);
    });
    //
    //

    // Edits Horizontales
    connect(red_hedit, &QLineEdit::editingFinished, this, [=]() {
        float value = red_hedit->text().toDouble();
        set_color(value, green, blue, alpha);
    });
    connect(green_hedit, &QLineEdit::editingFinished, this, [=]() {
        float value = green_hedit->text().toDouble();
        set_color(red, value, blue, alpha);
    });
    connect(blue_hedit, &QLineEdit::editingFinished, this, [=]() {
        float value = blue_hedit->text().toDouble();
        set_color(red, green, value, alpha);
    });
    connect(alpha_hedit, &QLineEdit::editingFinished, this, [=]() {
        float value = alpha_hedit->text().toDouble();
        set_color(red, green, blue, value);
    });
    //
    //

    connect(palette_button, &QPushButton::clicked, this,
            [this]() { toggle_sliders_colors(); });

    connect(mono_color_button, &QPushButton::clicked, this,
            [this]() { toggle_mono_color(); });
}
