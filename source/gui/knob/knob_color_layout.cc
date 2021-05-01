#include <knob_color.h>

void knob_color::setup_ui()
{
    layout->setMargin(0);
    layout->addWidget(init_space);

    mono_edit->setMaximumWidth(50);
    mono_edit->setText(QString::number(red));
    layout->addWidget(mono_edit);
    layout->addWidget(mono_slider);

    // Caja de colores separados
    {
        separate_colors_box_layout->setMargin(0);

        red_hedit->setObjectName("red_edit");
        separate_colors_box_layout->addWidget(red_hedit);

        green_hedit->setObjectName("green_edit");
        separate_colors_box_layout->addWidget(green_hedit);

        blue_hedit->setObjectName("blue_edit");
        separate_colors_box_layout->addWidget(blue_hedit);

        alpha_hedit->setObjectName("alpha_edit");
        separate_colors_box_layout->addWidget(alpha_hedit);

        separate_colors_box->hide();
        layout->addWidget(separate_colors_box);
    }

    // Caja de sliders de rgb
    {
        separate_colors_slider_layout->setMargin(0);
        separate_colors_slider_layout->setSpacing(5);

        // Red
        red_layout->setMargin(0);
        red_vedit->setMaximumWidth(50);
        red_vedit->setObjectName("red_edit");
        red_layout->addWidget(red_vedit);
        red_layout->addWidget(red_slider);
        red_slider->setObjectName("red_slider");
        separate_colors_slider_layout->addWidget(red_widget);
        //

        // Green
        green_layout->setMargin(0);
        green_vedit->setMaximumWidth(50);
        green_vedit->setObjectName("green_edit");
        green_layout->addWidget(green_vedit);
        green_slider->setObjectName("green_slider");
        green_layout->addWidget(green_slider);
        separate_colors_slider_layout->addWidget(green_widget);
        //

        // Blue
        blue_layout->setMargin(0);
        blue_vedit->setMaximumWidth(50);
        blue_vedit->setObjectName("blue_edit");
        blue_layout->addWidget(blue_vedit);
        blue_slider->setObjectName("blue_slider");
        blue_layout->addWidget(blue_slider);
        separate_colors_slider_layout->addWidget(blue_widget);
        //

        // Alpha
        alpha_layout->setMargin(0);
        alpha_vedit->setMaximumWidth(50);
        alpha_vedit->setObjectName("alpha_edit");
        alpha_layout->addWidget(alpha_vedit);
        alpha_slider->setObjectName("alpha_slider");
        alpha_layout->addWidget(alpha_slider);
        separate_colors_slider_layout->addWidget(alpha_widget);
        //
        separate_colors_slider->hide();
        layout->addWidget(separate_colors_slider);
    }

    //
    //

    picker_button->setObjectName("small_button");
    picker_button->setAutoFillBackground(true);
    picker_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    layout->addWidget(picker_button);

    palette_button->setObjectName("small_button");
    qt::set_icon(palette_button, "color_a", icon_size);
    layout->addWidget(palette_button);

    mono_color_button->setObjectName("small_button");
    mono_color_button->setText("4");
    layout->addWidget(mono_color_button);
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

    connect(palette_button, &QPushButton::clicked, this, [this]() { toggle_sliders_colors(); });

    connect(mono_color_button, &QPushButton::clicked, this, [this]() { toggle_mono_color(); });
}
