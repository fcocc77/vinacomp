#include <curve_curve.h>

curve::curve(QString _name)
    : name(_name)
{
}

curve::~curve()
{
    for (key_frame *key : keys)
        delete key;
}

key_frame *curve::add_key(int index, QPointF position, QColor color)
{
    key_frame *key = new key_frame(name, index, position, color);
    keys.push_back(key);

    return key;
}

