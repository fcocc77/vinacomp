#ifndef CURVE_UTILS_H
#define CURVE_UTILS_H

#include <QLineF>
#include <QPointF>

QPointF cubic_bezier(QPointF p1, QPointF p2, QPointF p3, QPointF p4,
                     float value);

QLineF handler_points(QPointF key, float left_angle, float right_angle,
                      float left_separation, float right_separation);

#endif // CURVE_UTILS_H
