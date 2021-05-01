#ifndef SLIDER_HPP
#define SLIDER_HPP

// #include <QSlider>
#include <QWidget>
#include <QPainter>
#include <QStyleOption>
#include <QResizeEvent>
#include <QLabel>

class slider : public QWidget
{
    Q_OBJECT
private:
    bool floating;
    float float_interval;

    float handler_percent;
    float min, max, default_value;

    bool centered_handler;
    bool out_range;

    QWidget *slider_center ;
    QLabel *handler;
    QLabel *handler_left;
    QLabel *handler_right;

    void refresh();
    void to_emmit_signal();
    float get_value_by_percent(float percent);
    float get_percent_by_value(float value);

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

public:
    slider(float _min = 0, float _max = 100, float _default_value = 0,
           bool _floating = true, bool centered_handler = false);
    ~slider();

    void set_value(float value);
    void set_default_value(float _default_value);

signals:
    void moved(float value);
};

#endif // SLIDER_HPP
