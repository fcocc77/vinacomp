#ifndef SLIDER_HPP
#define SLIDER_HPP

// #include <QSlider>
#include <QWidget>
#include <QPainter>
#include <QStyleOption>
#include <QResizeEvent>

class slider : public QWidget
{
    Q_OBJECT
private:
    bool floating;
    float float_interval;

    float handler_percent;
    float min, max, default_value;

    bool center_default_value;

    QWidget *slider_center, *handler;

    void refresh();
    void to_emmit_signal();
    float get_value_by_percent(float percent);
    float get_percent_by_value(float value);

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

public:
    slider(float _min = 0, float _max = 100, float _default_value = 1.0,
           bool _floating = true);
    ~slider();

    void set_min_max(float min, float max);
    void set_value(float value, bool emmit_signal = true);

signals:
    void moved(float value);
};

#endif // SLIDER_HPP
