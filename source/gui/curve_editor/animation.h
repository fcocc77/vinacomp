#ifndef ANIMATION_H
#define ANIMATION_H

#include <QJsonObject>
#include <QList>
#include <QString>

namespace anim
{
struct key_data
{
    int frame;
    float value;
    float left_angle;
    float right_angle;
    bool broken;
};

float get_value(QString curve, int frame);
QString update_curve(QString curve, float value, int frame);

key_data get_keyframe(QString frame_string);
QList<key_data> convert_curve(QString curve);

}; // namespace anim


#endif // ANIMATION_H
