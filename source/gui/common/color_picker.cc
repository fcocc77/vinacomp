#include <color_picker.h>
#include <util.h>

color_picker::color_picker()
    : layout(new QVBoxLayout(this))
{
    color_box *_color_box = new color_box;
    rainbow_box *rainbow = new rainbow_box;
    rainbow->setMaximumHeight(50);

    connect(rainbow, &rainbow_box::changed, _color_box, &color_box::set_color);

    layout->addWidget(_color_box);
    layout->addWidget(rainbow);
}

color_picker::~color_picker() {}

void color_box::set_color(QColor _color)
{
    color = _color;
    update();
}

void color_box::paintEvent(QPaintEvent *event)
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
}

void rainbow_box::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QLinearGradient ramp(0, 0, width(), 0);
    float section = 1.0 / 6;

    ramp.setColorAt(0.0, Qt::red);
    ramp.setColorAt(section, Qt::magenta);
    ramp.setColorAt(section * 2, Qt::blue);
    ramp.setColorAt(section * 3, Qt::cyan);
    ramp.setColorAt(section * 4, Qt::green);
    ramp.setColorAt(section * 5, Qt::yellow);
    ramp.setColorAt(1.0, Qt::red);

    painter.fillRect(0, 0, width(), height(), QBrush(ramp));
}

void rainbow_box::emmit_signal(float pos_x)
{
    if (pos_x < 0)
        pos_x = 0;

    if (pos_x >= width() - 1)
        pos_x = width() - 1;

    QPixmap qPix = this->grab();
    QImage image(qPix.toImage());
    QColor color(image.pixel(pos_x, 0));

    changed(color);
}

void rainbow_box::mousePressEvent(QMouseEvent *event)
{
    emmit_signal(event->pos().x());
}

void rainbow_box::mouseMoveEvent(QMouseEvent *event)
{
    emmit_signal(event->pos().x());
}
