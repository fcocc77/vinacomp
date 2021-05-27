#include <color_picker.h>
#include <util.h>

color_picker::color_picker()
    : layout(new QVBoxLayout(this))
    , hue(0)
    , sat(0)
    , level(0)
{
    layout->setMargin(0);

    setMinimumHeight(100);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

color_picker::~color_picker() {}

void color_picker::set_color(QColor _color)
{
    color = _color;
    update();
}

void color_picker::update_picker()
{
    QPointF pos = circle_position;

    if (pos.x() < 0)
        pos.setX(0);

    if (pos.x() >= width() - 1)
        pos.setX(width() - 1);

    if (pos.y() < 0)
        pos.setY(0);

    if (pos.y() >= height() - 1)
        pos.setY(height() - 1);

    QPixmap qPix = this->grab();
    QImage image(qPix.toImage());
    QColor color = image.pixel(pos.x(), pos.y());

    rgb_to_hsl(color, hue, sat, level);
    changed(color, hue, sat, level);
    update();
}

void color_picker::set_circle_position(float sat, float level)
{
    circle_position = {width() * sat, height() * (1 - level)};
    update();
}

void color_picker::set_hsl(float _hue, float _sat, float _level)
{
    hue = _hue;
    sat = _sat;
    level = _level;

    set_circle_position(sat, level);
    set_color(hsl_to_rgb(hue, 1, 1));
}

QColor color_picker::hsl_to_rgb(float H, float S, float L)
{
    float s = S;
    float v = L;
    float C = s * v;
    float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
    float m = v - C;
    float r, g, b;

    if (H >= 0 && H < 60)
        r = C, g = X, b = 0;
    else if (H >= 60 && H < 120)
        r = X, g = C, b = 0;
    else if (H >= 120 && H < 180)
        r = 0, g = C, b = X;
    else if (H >= 180 && H < 240)
        r = 0, g = X, b = C;
    else if (H >= 240 && H < 300)
        r = X, g = 0, b = C;
    else
        r = C, g = 0, b = X;

    int R = (r + m) * 255;
    int G = (g + m) * 255;
    int B = (b + m) * 255;

    return {R, G, B};
}

void color_picker::rgb_to_hsl(QColor rgb, float &H, float &S, float &L)
{
    float red = (float)rgb.red() / 255.0;
    float green = (float)rgb.green() / 255.0;
    float blue = (float)rgb.blue() / 255.0;

    float fCMax = max(max(red, green), blue);
    float fCMin = min(min(red, green), blue);
    float fDelta = fCMax - fCMin;

    if (fDelta > 0)
    {
        if (fCMax == red)
            H = 60 * (fmod(((green - blue) / fDelta), 6));
        else if (fCMax == green)
            H = 60 * (((blue - red) / fDelta) + 2);
        else if (fCMax == blue)
            H = 60 * (((red - green) / fDelta) + 4);

        if (fCMax > 0)
            S = fDelta / fCMax;
        else
            S = 0;

        L = fCMax;
    }
    else
    {
        H = 0;
        S = 0;
        L = fCMax;
    }

    if (H < 0)
        H = 360 + H;
}

void color_picker::mousePressEvent(QMouseEvent *event)
{
    circle_position = event->pos();
    update_picker();
}

void color_picker::mouseMoveEvent(QMouseEvent *event)
{
    float x = event->x();
    float y = event->y();

    if (x > width())
        x = width();
    else if (x < 0)
        x = 0;

    if (y > height())
        y = height();
    else if (y < 0)
        y = 0;

    circle_position = {x, y};
    update_picker();
}

void color_picker::resizeEvent(QResizeEvent *event)
{
    set_circle_position(sat, level);
}

void color_picker::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QLinearGradient ramp_x(0, 0, width(), 0);
    ramp_x.setColorAt(0.0, Qt::white);
    ramp_x.setColorAt(1.0, color);

    QLinearGradient ramp_y(0, 0, 0, height());
    ramp_y.setColorAt(0.0, Qt::white);
    ramp_y.setColorAt(1.0, Qt::black);

    QBrush brush_x(ramp_x);
    QBrush brush_y(ramp_y);

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(0, 0, width(), height(), brush_x);
    painter.setCompositionMode(QPainter::CompositionMode_Multiply);
    painter.fillRect(0, 0, width(), height(), brush_y);

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    float radius = 10;
    painter.setPen(Qt::white);
    painter.drawEllipse(circle_position, radius, radius);
}
