#include <curve_key_frame.hpp>

key_frame::key_frame(QString _curve_name, int _index)
    : curve_name(_curve_name),
      index(_index)

{
}

key_frame::~key_frame()
{
}

QPointF key_frame::pos()
{
    return position;
}

float key_frame::x()
{
    return position.x();
}

float key_frame::y()
{
    return position.y();
}

void key_frame::set_pos(QPointF _position)
{
    position = _position;
}

float key_frame::get_angle()
{
    return angle;
}

void key_frame::set_angle(float _angle)
{
    angle = _angle;
}

bool key_frame::selected()
{
    return _selected;
}

void key_frame::select(bool select)
{
    _selected = select;
}

bool key_frame::is_null()
{
    if (index < 0)
        return true;
    else
        return false;
}

int key_frame::get_index()
{
    return index;
}

QString key_frame::get_curve()
{
    return curve_name;
}

float key_frame::exaggeration()
{
    return _exaggeration;
}

void key_frame::set_exaggeration(float __exaggeration)
{
    _exaggeration = __exaggeration;
}