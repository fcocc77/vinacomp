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

float get_valuef(QString curve, int frame);
int get_value(QString curve, int frame);

QString update_curve(QString curve, int value, int frame);
QString update_curvef(QString curve, int value, float frame);

QList<key_data> convert_curve(QString curve);
}; // namespace anim

#endif // ANIMATION_H
