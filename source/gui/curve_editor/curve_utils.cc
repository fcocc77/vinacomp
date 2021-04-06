#include <curve_utils.h>
#include <math.h>

QPointF cubic_bezier(QPointF p1, QPointF p2, QPointF p3, QPointF p4,
                     float value)
{
    // Algoritmo bezier
    QPointF L1 = ((1 - value) * p1) + (value * p2);
    QPointF L2 = ((1 - value) * p2) + (value * p3);
    QPointF L3 = ((1 - value) * p3) + (value * p4);

    QPointF Q1 = ((1 - value) * L1) + (L2 * value);
    QPointF Q2 = ((1 - value) * L2) + (L3 * value);

    return ((1 - value) * Q1) + (Q2 * value);
    //
}

QLineF handler_points(QPointF key, float left_angle, float right_angle,
                      float left_separation, float right_separation)
{
    float left_point_x = key.x() - left_separation;
    float right_point_x = key.x() + right_separation;

    QPointF left_point = {left_point_x, key.y()};
    QPointF right_point = {right_point_x, key.y()};

    // genera el punto vertical infinito donde apunta el manejador
    float left_tangent = tan(left_angle * M_PI / 180);
    float right_tangent = tan(-right_angle * M_PI / 180);

    left_point = {left_point.x(),
                  left_point.y() + (left_tangent * left_separation)};

    right_point = {right_point.x(),
                   right_point.y() - (right_tangent * right_separation)};
    //

    return {left_point, right_point};
}

