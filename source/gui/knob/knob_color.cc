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
    red_slider = new knob_color_slider(min, max, default_red, centered_handler, "R");
    green_slider =
        new knob_color_slider(min, max, default_green, centered_handler, "G");
    blue_slider =
        new knob_color_slider(min, max, default_blue, centered_handler, "B");
    alpha_slider =
        new knob_color_slider(min, max, default_alpha, centered_handler, "A");

    hue_slider = new knob_color_slider(0, 360, 0, centered_handler, "H º");
    sat_slider = new knob_color_slider(0, 1, 1, centered_handler, "S %");
    level_slider = new knob_color_slider(0, 1, 1, centered_handler, "L %");

    hue_slider->set_colored_slider("hue");
    sat_slider->set_colored_slider("sat");
    level_slider->set_colored_slider("level");

    color_sample_button = new button(this);
    palette_button = new button(this);
    mono_color_button = new button(this);
    rgb_button = new button(this);
    hsl_button = new button(this);
    picker_button = new button(this);

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

    mono_color_button->set_checked(mono_color);
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

    update();
}

void knob_color::toogle_advanced_options()
{
    advanced_options = !advanced_options;

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

    mono_color_button->set_disable(advanced_options);
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

void knob_color::toogle_color_picker_option()
{
    color_picker_option = !color_picker_option;
    _color_picker->setVisible(color_picker_option);
}

QColor knob_color::qcolor_from_rgb(float r, float g, float b, float a)
{
    if (r < 0)
        r = 0;
    else if (r > 1)
        r = 1;

    if (g < 0)
        g = 0;
    else if (g > 1)
        g = 1;

    if (b < 0)
        b = 0;
    else if (b > 1)
        b = 1;

    else if (a < 0)
        a = 0;
    if (a > 1)
        a = 1;

    return QColor{int(r * 255.0), int(g * 255.0), int(b * 255.0),
                  int(a * 255.0)};
}

void knob_color::rgb_from_qcolor(QColor color, float *r, float *g, float *b,
                                 float *a)
{
    *r = (float)color.red() / 255.0;
    *g = (float)color.green() / 255.0;
    *b = (float)color.blue() / 255.0;

    if (a)
        *a = (float)color.alpha() / 255.0;
}

void knob_color::update_hsl_sliders()
{
    float h, s, l;

    color_picker::rgb_to_hsl(qcolor_from_rgb(red, green, blue), h, s, l);

    hue_slider->set_value(h, false);
    sat_slider->set_value(s, false);
    level_slider->set_value(l, false);

    hue_slider->set_hsl(h, s, l);
    sat_slider->set_hsl(h, s, l);
    level_slider->set_hsl(h, s, l);
}

void knob_color::update_color_picker()
{
    float h = hue_slider->get_value();
    float s = sat_slider->get_value();
    float l = level_slider->get_value();

    _color_picker->set_hsl(h, s, l);

    hue_slider->set_hsl(h, s, l);
    sat_slider->set_hsl(h, s, l);
    level_slider->set_hsl(h, s, l);
}

void knob_color::update_rgb_sliders()
{
    red_slider->set_value(red, false);
    green_slider->set_value(green, false);
    blue_slider->set_value(blue, false);
    alpha_slider->set_value(alpha, false);
}

void knob_color::set_color(QColor color, bool emmit_signal, bool update_sliders)
{
    float r, g, b, a;
    rgb_from_qcolor(color, &r, &g, &b, &a);
    set_color(r, g, b, a, emmit_signal, update_sliders);
}

void knob_color::set_hsl(float hue, float sat, float level, bool emmit_signal,
                         bool update_sliders)
{
    set_color(color_picker::hsl_to_rgb(hue, sat, level), emmit_signal,
              update_sliders);
}

void knob_color::set_color(float _red, float _green, float _blue, float _alpha,
                           bool emmit_signal, bool update_sliders)
{
    red = _red;
    green = _green;
    blue = _blue;
    alpha = _alpha;

    if (red == green && red == blue && red == alpha)
        mono_slider->set_value(red);

    red_hedit->setText(QString::number(red));
    green_hedit->setText(QString::number(green));
    blue_hedit->setText(QString::number(blue));
    alpha_hedit->setText(QString::number(alpha));

    if (update_sliders)
    {
        update_hsl_sliders();
        update_rgb_sliders();
        update_color_picker();
    }

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
