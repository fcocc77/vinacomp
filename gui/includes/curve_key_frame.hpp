#ifndef CURVE_KEY_FRAME_HPP
#define CURVE_KEY_FRAME_HPP

#include <QObject>
#include <QPointF>

class key_frame
{
private:
    QPointF position;
    float angle = 0;
    bool _selected = false;
    float _exaggeration = 0.3; // 0.0 - 1.0
    QString curve_name;
    int index = -1;

    // interpolacion
    int interpolation = 0;
    // 0: Linear
    // 1: Horizontal
    // 2: Smooth
    // 3: Break
    // 4: Custom

public:
    key_frame(QString curve_name = "", int index = -1);
    ~key_frame();

    QPointF pos();
    float x();
    float y();
    void set_pos(QPointF _position);
    float get_angle();
    void set_angle(float _angle);
    bool selected();
    void select(bool select);
    bool is_null();
    int get_index();
    QString get_curve();
    float exaggeration();
    void set_exaggeration(float _exaggeration);
};

#endif //CURVE_KEY_FRAME_HPP