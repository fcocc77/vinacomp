#include <curve_key_frame.h>
#include <util.h>

key_frame::key_frame(QString _curve_name, int _index, QPointF _position, QColor _color)

    : curve_name(_curve_name)
    , index(_index)
    , position(_position)
    , last_position(_position)
    , color(_color)
    ,

    left_angle(0)
    , right_angle(0)
    , _selected(false)
    , _exaggeration(0.3)
    , broken(false)
    , _left_interpolation(2)
    , _right_interpolation(2)
{
}

key_frame::~key_frame() {}

void key_frame::set_interpolation(int left_number, int right_number)
{
    // 0: Linear
    // 1: Horizontal
    // 2: Smooth
    // 3: Broken
    // 4: Custom

    auto set = [=](int number, int handler) {
        if (handler == 0)
            _left_interpolation = number;
        if (handler == 1)
            _right_interpolation = number;

        if (number == 0)
            this->set_broken(true);

        else if (number == 1)
        {
            if (handler == 0)
                this->set_left_angle(0);

            if (handler == 1)
                this->set_right_angle(0);

            this->set_broken(false);
        }
        else if (number == 2)
            this->set_broken(false);

        else if (number == 3)
            this->set_broken(true);
    };

    if (left_number > -1)
        set(left_number, 0);

    if (right_number > -1)
        set(right_number, 1);
}
