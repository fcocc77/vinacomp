#ifndef ANIMATION_H
#define ANIMATION_H

#include <QJsonObject>
#include <QList>
#include <QString>

#include <curve_curve.h>

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

float get_value(QString curve, int frame, bool *is_keyframe = nullptr);

key_data get_keyframe(QString frame_string);
QList<key_data> convert_curve(QString curve);
QString set_keyframe(QString curve, int frame, bool calc_value = true,
                     float value = 0, bool force = false);

QString curve_data_to_string(QList<key_data> _curve);
QString curve_to_string(curve *_curve);

QString delete_keys_from_curve(QString curve, QList<int> indexs_to_delete);

}; // namespace anim


#endif // ANIMATION_H
