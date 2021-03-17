#ifndef CURVE_KEY_FRAME_HPP
#define CURVE_KEY_FRAME_HPP

#include <QObject>
#include <QPointF>

#include <util.h>

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
    key_frame( QString _curve_name, int _index, QPointF _position, QColor _color );
    ~key_frame();

    QPointF pos();
    float x();
    float y();
    void set_pos( QPointF _position );

    float get_left_angle();
    float get_right_angle();

    void set_left_angle( float _angle );
    void set_right_angle( float _angle );

    bool selected();
    bool is_broken();
    bool set_broken( bool _broken );
    void set_interpolation( int left_number, int right_number );
    int right_interpolation();
    int left_interpolation();

    QColor get_color();

    void select( bool select );
    int get_index();
    QString get_curve();
    float exaggeration();
    void set_exaggeration( float _exaggeration );
    void stamp_position();
    void restore_position();
    QPointF get_last_position();
};

#endif // CURVE_KEY_FRAME_HPP