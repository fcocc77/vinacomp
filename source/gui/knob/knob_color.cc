#include <knob_color.h>

knob_color::knob_color(knob_props props, float min, float max, float r, float g,
                       float b, float a, bool centered_handler)
    : knob(props)
    , red(r)
    , green(g)
    , blue(b)
    , alpha(a)
    , default_red(r)
    , default_green(g)
    , default_blue(b)
    , default_alpha(a)

    , mono_color(false)
    , advanced_options(false)
    , hsl_option(false)
    , rgb_option(true)
    , hue_option(false)
    , color_picker_option(false)
{
    this->setObjectName("knob_color");

    main_layout = new QHBoxLayout(this);

    top_widget = new QWidget(this);
    top_layout = new QHBoxLayout(top_widget);
    this->set_knob_layout(top_layout);

    content_widget = new QWidget(this);
    content_layout = new QVBoxLayout(content_widget);

    _color_picker = new color_picker;

    separate_colors_box = new QWidget(this);
    separate_colors_box_layout = new QHBoxLayout(separate_colors_box);
    rgba_box = new QWidget(this);
    rgba_box_layout = new QVBoxLayout(rgba_box);
    hsl_box = new QWidget(this);
    hsl_box_layout = new QVBoxLayout(hsl_box);

    red_hedit = new QLineEdit(this);
    green_hedit = new QLineEdit(this);
    blue_hedit = new QLineEdit(this);
    alpha_hedit = new QLineEdit(this);

    mono_slider =
        new knob_color_slider(min, max, default_red, centered_handler);
    red_slider = new knob_color_slider(min, max, default_red, centered_handler);
    green_slider =
        new knob_color_slider(min, max, default_green, centered_handler);
    blue_slider =
        new knob_color_slider(min, max, default_blue, centered_handler);
    alpha_slider =
        new knob_color_slider(min, max, default_alpha, centered_handler);

    hue_slider = new knob_color_slider(0, 360, 0, centered_handler);
    sat_slider = new knob_color_slider(0, 1, 1, centered_handler);
    level_slider = new knob_color_slider(0, 1, 1, centered_handler);

    color_sample_button = new QPushButton(this);
    palette_button = new QPushButton(this);
    mono_color_button = new QPushButton(this);
    hue_button = new QPushButton(this);
    rgb_button = new QPushButton(this);
    hsl_button = new QPushButton(this);
    picker_button = new QPushButton(this);

    connections();
    setup_ui();
    set_init_color(red, green, blue, alpha);
}

knob_color::~knob_color()
{
    delete mono_slider;

    delete red_hedit;
    delete green_hedit;
    delete blue_hedit;
    delete alpha_hedit;

    delete red_slider;
    delete green_slider;
    delete blue_slider;
    delete alpha_slider;

    delete hue_slider;
    delete sat_slider;
    delete level_slider;

    delete color_sample_button;
    delete palette_button;
    delete mono_color_button;
    delete hue_button;
    delete rgb_button;
    delete hsl_button;
    delete picker_button;

    delete separate_colors_box_layout;
    delete rgba_box_layout;
    delete separate_colors_box;
    delete rgba_box;
    delete hsl_box_layout;
    delete hsl_box;

    delete _color_picker;

    delete main_layout;
}

void knob_color::set_init_color(float red, float green, float blue, float alpha)
{
    // si los colores son todos iguale activa el 'knob' mono cromatico
    if (red == green && red == blue && red == alpha)
    {
        mono_color = false;
        set_visible_mono_color(true, false);
    }
    else
    {
        mono_color = true;
        set_visible_mono_color(false, false);
    }
    //
    //
    set_color(red, green, blue, alpha, false);
}

void knob_color::restore_default()
{
    knob::restore_default();
    set_color(default_red, default_green, default_blue, default_alpha, false);
}

void knob_color::restore_param()
{
    QJsonArray param_value = get_param_value().toArray();
    QJsonArray _default = get_default().toArray();

    default_red = _default.at(0).toDouble();
    default_green = _default.at(1).toDouble();
    default_blue = _default.at(2).toDouble();
    default_alpha = _default.at(3).toDouble();

    red = param_value.at(0).toDouble();
    green = param_value.at(1).toDouble();
    blue = param_value.at(2).toDouble();
    alpha = param_value.at(3).toDouble();

    mono_slider->set_default_value(default_red);

    red_slider->set_default_value(default_red);
    green_slider->set_default_value(default_green);
    blue_slider->set_default_value(default_blue);
    alpha_slider->set_default_value(default_alpha);

    set_init_color(red, green, blue, alpha);
}

void knob_color::update()
{
    set_color(red, green, blue, alpha);
}

void knob_color::set_visible_mono_color(bool visible, bool emmit_signal)
{
    if (advanced_options)
    {
        visible = false;
        separate_colors_box->setVisible(false);
    }
    else
    {
        separate_colors_box->setVisible(!visible);
    }

    mono_slider->setVisible(visible);

    if (visible)
    {
        // promedia todos los colores para el mono color
        float color = (red + green + blue) / 3.0;

        mono_slider->set_value(color);

        set_color(color, color, color, color, emmit_signal);
    }

    qt::set_property(mono_color_button, "active", !visible);
    if (emmit_signal)
        update();
}

void knob_color::toggle_mono_color()
{
    mono_color = !mono_color;
    set_visible_mono_color(!mono_color);
}

void knob_color::set_visible_rgba_box(bool visible)
{
    rgba_box->setVisible(visible);

    // al activar el cuadro se colors sliders se desabilita el mono color
    mono_color = !visible;
    set_visible_mono_color(false);
    //

    qt::set_property(palette_button, "active", visible);
    qt::set_property(mono_color_button, "disable", visible);
    update();
}

void knob_color::toogle_advanced_options()
{
    advanced_options = !advanced_options;

    hue_button->setVisible(advanced_options);
    rgb_button->setVisible(advanced_options);
    hsl_button->setVisible(advanced_options);
    picker_button->setVisible(advanced_options);

    if (!advanced_options)
    {
        set_visible_rgba_box(false);
        hsl_box->setVisible(false);
        _color_picker->setVisible(false);
    }
    else
    {
        _color_picker->setVisible(color_picker_option);
        set_visible_rgba_box(rgb_option);
        hsl_box->setVisible(hsl_option);
    }
}

void knob_color::toogle_rgb_option()
{
    rgb_option = !rgb_option;
    set_visible_rgba_box(rgb_option);
}

void knob_color::toogle_hsl_option()
{
    hsl_option = !hsl_option;
    hsl_box->setVisible(hsl_option);
}

void knob_color::toogle_hue_option()
{
    hue_option = !hue_option;
}

void knob_color::toogle_color_picker_option()
{
    color_picker_option = !color_picker_option;
    _color_picker->setVisible(color_picker_option);
}

void knob_color::set_color(float _red, float _green, float _blue, float _alpha,
                           bool emmit_signal)
{
    red = _red;
    green = _green;
    blue = _blue;
    alpha = _alpha;

    if (red == green && red == blue && red == alpha)
        mono_slider->set_value(red);

    red_hedit->setText(QString::number(red));
    red_slider->set_value(red);

    green_hedit->setText(QString::number(green));
    green_slider->set_value(green);

    blue_hedit->setText(QString::number(blue));
    blue_slider->set_value(blue);

    alpha_hedit->setText(QString::number(alpha));
    alpha_slider->set_value(alpha);

    if (emmit_signal)
    {
        changed(red, green, blue, alpha); // Signal
        update_value(QJsonArray{red, green, blue, alpha});
    }

    if (_red < 0)
        _red = 0;
    if (_red > 1)
        _red = 1;

    if (_green < 0)
        _green = 0;
    if (_green > 1)
        _green = 1;

    if (_blue < 0)
        _blue = 0;
    if (_blue > 1)
        _blue = 1;

    // Cambia el color del boton de picker
    QString __red = QString::number(_red * 255);
    QString __green = QString::number(_green * 255);
    QString __blue = QString::number(_blue * 255);

    color_sample_button->setStyleSheet("background: rgb(" + __red + "," +
                                       __green + "," + __blue + ");");
    color_sample_button->update();
}

void knob_color::disable_alpha()
{
    alpha_slider->hide();
    alpha_hedit->hide();
    mono_color_button->setText("3");
}
