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

void key_frame::stamp_position()
{
    // guarda una posicion anterior que es
    // independiente de la posicion actual
    last_position = position;
}

void key_frame::restore_position()
{
    // restaura la posicion guardada en 'last_position'
    position = last_position;
}

QPointF key_frame::get_last_position()
{
    return last_position;
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

float key_frame::get_left_angle()
{
    return left_angle;
}

float key_frame::get_right_angle()
{
    return right_angle;
}

void key_frame::set_left_angle(float _angle)
{
    left_angle = _angle;
}

void key_frame::set_right_angle(float _angle)
{
    right_angle = _angle;
}

bool key_frame::selected()
{
    return _selected;
}

void key_frame::select(bool select)
{
    _selected = select;
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

bool key_frame::is_break()
{
    return break_handler;
}

bool key_frame::set_break(bool _break)
{
    break_handler = _break;
}

void key_frame::set_interpolation(int left_number, int right_number)
{
    // 0: Linear
    // 1: Horizontal
    // 2: Smooth
    // 3: Break
    // 4: Custom

    auto set = [=](int number, int handler) {
        if (handler == 0)
            _left_interpolation = number;
        if (handler == 1)
            _right_interpolation = number;

        if (number == 0)
            this->set_break(true);

        else if (number == 1)
        {
            if (handler == 0)
                this->set_left_angle(0);

            if (handler == 1)
                this->set_right_angle(0);

            this->set_break(false);
        }
        else if (number == 2)
            this->set_break(false);

        else if (number == 3)
            this->set_break(true);

    };

    if (left_number > -1)
        set(left_number, 0);

    if (right_number > -1)
        set(right_number, 1);
}

int key_frame::right_interpolation()
{
    return _right_interpolation;
}

int key_frame::left_interpolation()
{
    return _left_interpolation;
}