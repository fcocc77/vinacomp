#ifndef CURVE_H
#define CURVE_H

#include <curve_key_frame.h>

class curve
{

private:
    QString name;
    QList<key_frame *> keys;

public:
    curve(QString name);
    ~curve();

    key_frame *add_key(int index, QPointF position, QColor color);
    inline key_frame *get_previous_key(key_frame *key) const;
    inline key_frame *get_next_key(key_frame *key) const;

    inline QList<key_frame *> get_keys() const;
    inline QString get_name() const;
};

inline QString curve::get_name() const
{
    return name;
}

inline QList<key_frame *> curve::get_keys() const
{
    return keys;
}

inline key_frame *curve::get_previous_key(key_frame *key) const
{
    return keys.value(key->get_index() - 1);
}

inline key_frame *curve::get_next_key(key_frame *key) const
{
    return keys.value(key->get_index() + 1);
}

#endif // CURVE_H
