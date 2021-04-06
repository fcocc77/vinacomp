#ifndef CURVE_KEY_FRAME_HPP
#define CURVE_KEY_FRAME_HPP

#include <QObject>
#include <QPointF>
#include <QColor>

class key_frame : public QObject
{
private:
    QPointF position;
    QPointF last_position;
    float left_angle;
    float right_angle;
    bool _selected;
    float _exaggeration;
    QString curve_name;
    int index;
    bool broken;
    QColor color;

    int _left_interpolation;
    int _right_interpolation;

public:
    key_frame(QString _curve_name, int _index, QPointF _position, QColor _color);
    ~key_frame();

    inline QPointF pos();
    inline float x();
    inline float y();
    inline void set_pos(QPointF _position);

    inline float get_left_angle();
    inline float get_right_angle();

    inline void set_left_angle(float _angle);
    inline void set_right_angle(float _angle);

    inline bool selected();
    inline bool is_broken();
    inline bool set_broken(bool _broken);
    void set_interpolation(int left_number, int right_number);
    inline int right_interpolation();
    inline int left_interpolation();

    inline QColor get_color();

    inline void select(bool select);
    inline int get_index();
    inline QString get_curve();
    inline float exaggeration();
    inline void set_exaggeration(float _exaggeration);
    inline void stamp_position();
    inline void restore_position();
    inline QPointF get_last_position();
};

inline QPointF key_frame::pos()
{
    return position;
}

inline void key_frame::stamp_position()
{
    // guarda una posicion anterior que es
    // independiente de la posicion actual
    last_position = position;
}

inline void key_frame::restore_position()
{
    // restaura la posicion guardada en 'last_position'
    position = last_position;
}

inline QPointF key_frame::get_last_position()
{
    return last_position;
}

inline float key_frame::x()
{
    return position.x();
}

inline float key_frame::y()
{
    return position.y();
}

inline void key_frame::set_pos(QPointF _position)
{
    position = _position;
}

inline float key_frame::get_left_angle()
{
    return left_angle;
}

inline float key_frame::get_right_angle()
{
    return right_angle;
}

inline void key_frame::set_left_angle(float _angle)
{
    left_angle = _angle;
}

inline void key_frame::set_right_angle(float _angle)
{
    right_angle = _angle;
}

inline bool key_frame::selected()
{
    return _selected;
}

inline void key_frame::select(bool select)
{
    _selected = select;
}

inline int key_frame::get_index()
{
    return index;
}

inline QString key_frame::get_curve()
{
    return curve_name;
}

inline float key_frame::exaggeration()
{
    return _exaggeration;
}

inline void key_frame::set_exaggeration(float __exaggeration)
{
    _exaggeration = __exaggeration;
}

inline bool key_frame::is_broken()
{
    return broken;
}

inline bool key_frame::set_broken(bool _broken)
{
    broken = _broken;
}

inline int key_frame::right_interpolation()
{
    return _right_interpolation;
}

inline int key_frame::left_interpolation()
{
    return _left_interpolation;
}

inline QColor key_frame::get_color()
{
    return color;
}

#endif // CURVE_KEY_FRAME_HPP
