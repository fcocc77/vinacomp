#ifndef SLIDER_HPP
#define SLIDER_HPP

#include <QSlider>

class slider : public QSlider
{
    Q_OBJECT
private:
    bool floating;
    float float_interval;
    bool emmit_signal;

public:
    slider(float _min = 0, float _max = 100, float default_value = 0, bool _floating = true);
    ~slider();

    void set_value(float value, bool emmit_signal = true);

protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void moved(float value);
};

#endif // SLIDER_HPP
