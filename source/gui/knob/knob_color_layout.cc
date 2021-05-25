#include <knob_color.h>

void knob_color::setup_ui()
{
    top_layout->setMargin(0);
    content_layout->setMargin(0);
    main_layout->setMargin(0);

    // Caja de colores separados
    separate_colors_box_layout->setMargin(0);

    red_hedit->setObjectName("red_edit");
    green_hedit->setObjectName("green_edit");
    blue_hedit->setObjectName("blue_edit");
    alpha_hedit->setObjectName("alpha_edit");

    // Caja de sliders de rgb
    separate_colors_slider_layout->setMargin(0);
    separate_colors_slider_layout->setSpacing(5);

    red_slider->setObjectName("red_slider");
    green_slider->setObjectName("green_slider");
    blue_slider->setObjectName("blue_slider");
    alpha_slider->setObjectName("alpha_slider");
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

    // Visibilidad Inicial
    hue_button->hide();
    rgb_button->hide();
    hsl_button->hide();
    picker_button->hide();
    _color_picker->hide();
    separate_colors_box->hide();
    separate_colors_slider->hide();

    // Aliniamiento de Layouts
    content_layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    top_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    separate_colors_slider->setSizePolicy(QSizePolicy::Expanding,
                                          QSizePolicy::Fixed);

    // Layout
    separate_colors_box_layout->addWidget(red_hedit);
    separate_colors_box_layout->addWidget(green_hedit);
    separate_colors_box_layout->addWidget(blue_hedit);
    separate_colors_box_layout->addWidget(alpha_hedit);

    separate_colors_slider_layout->addWidget(red_slider);
    separate_colors_slider_layout->addWidget(green_slider);
    separate_colors_slider_layout->addWidget(blue_slider);
    separate_colors_slider_layout->addWidget(alpha_slider);

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
    content_layout->addWidget(_color_picker);

    main_layout->addWidget(init_space);
    main_layout->addWidget(content_widget);
}

void knob_color::connections()
{
    connect(mono_slider, &knob_color_slider::changed, this,
            [=](float value) { set_color(value, value, value, value); });

    connect(red_slider, &knob_color_slider::changed, this,
            [=](float value) { set_color(value, green, blue, alpha); });

    connect(green_slider, &knob_color_slider::changed, this,
            [=](float value) { set_color(red, value, blue, alpha); });

    connect(blue_slider, &knob_color_slider::changed, this,
            [=](float value) { set_color(red, green, value, alpha); });

    connect(alpha_slider, &knob_color_slider::changed, this,
            [=](float value) { set_color(red, green, blue, value); });


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
            [this]() { toogle_advanced_options(); });

    connect(mono_color_button, &QPushButton::clicked, this,
            [this]() { toggle_mono_color(); });

    connect(picker_button, &QPushButton::clicked, this,
            [this]() { toogle_color_picker_option(); });

    connect(rgb_button, &QPushButton::clicked, this,
            [this]() { toogle_rgb_option(); });

    connect(hsl_button, &QPushButton::clicked, this,
            [this]() { toogle_hsl_option(); });

    connect(hue_button, &QPushButton::clicked, this,
            [this]() { toogle_hue_option(); });
}
