#include <animation.h>
#include <curve_utils.h>
#include <util.h>

float anim::get_value(QString curve, int frame, bool *is_keyframe)
{
    if (is_keyframe)
        (*is_keyframe) = false;

    if (curve.isEmpty())
        return 0;

    // Extraer el valor del frame, de la curva
    curve.remove(0, 1);

    QStringList frames = curve.split('f');

    int next_index = -1;
    for (int i = 0; i < frames.count(); i++)
    {
        QStringList fdata = frames[i].split(' ');
        int _frame = fdata[0].toInt();

        // si el frame a buscar existe en un keyframe, retorna
        // inmediatamente el valor.
        if (frame == _frame)
        {
            if (is_keyframe)
                (*is_keyframe) = true;
            return fdata[1].toFloat();
        }
        //

        // obtiene el index del siguiente keyframe
        if (frame < _frame)
        {
            next_index = i;
            break;
        }
    }

    int prev_index = next_index - 1;

    // si no existe el frame previo o el siguiete, retorna el
    // primer o ultimo valor de la curva
    if (prev_index < 0)
        return get_keyframe(frames[0]).value;

    if (next_index < 0)
        return get_keyframe(frames[frames.count() - 1]).value;
    //

    key_data prev_key, next_key;

    prev_key = get_keyframe(frames[prev_index]);
    next_key = get_keyframe(frames[next_index]);

    QPointF prev_key_pos = {(float)prev_key.frame, prev_key.value};
    QPointF next_key_pos = {(float)next_key.frame, next_key.value};

    // Manejadores del frame previo
    float prev_left_separation = 0;
    float prev_right_separation = 0;

    QLineF prev_handler =
        handler_points(prev_key_pos, prev_key.left_angle, prev_key.right_angle,
                       prev_left_separation, prev_right_separation);

    QPointF src_handler = prev_handler.p2();
    //

    // Manejadores del frame siguiente
    float next_left_separation = 0;
    float next_right_separation = 0;

    QLineF next_handler =
        handler_points(next_key_pos, next_key.left_angle, next_key.right_angle,
                       next_left_separation, next_right_separation);

    QPointF dst_handler = next_handler.p2();
    //

    float value =
        (frame - prev_key_pos.x()) / (next_key_pos.x() - prev_key_pos.x());

    QPointF bezier = cubic_bezier(prev_key_pos, src_handler, dst_handler,
                                  next_key_pos, value);

    return bezier.y();
}

QString anim::set_keyframe(QString curve, int frame, bool calc_value, float value)
{
    bool keyframe_exist = false;
    float _value = get_value(curve, frame, &keyframe_exist);

    if (keyframe_exist)
        return curve;

    if (calc_value)
        value = _value;

    key_data new_key;
    new_key.frame = frame;
    new_key.value = value;

    auto keys = convert_curve(curve);

    // encuentra el index donde corresponda insertar el nuevo keyframe
    int index_to_insert = 0;
    for (key_data key : keys)
    {
        if (key.frame > frame)
            break;
        index_to_insert++;
    }
    //

    keys.insert(index_to_insert, new_key);

    return curve_data_to_string(keys);
}

anim::key_data anim::get_keyframe(QString frame_string)
{
    QStringList fdata = frame_string.split(' ');
    anim::key_data key;

    key.frame = fdata[0].toInt();

    if (fdata.count() > 1)
        key.value = fdata[1].toFloat();
    else
        key.value = 0;

    if (fdata.count() > 3)
    {
        key.left_angle = fdata[2].replace('l', "").toFloat();
        key.right_angle = fdata[3].replace('r', "").toFloat();
    }
    else
    {
        key.left_angle = 0;
        key.right_angle = 0;
    }

    return key;
}

QList<anim::key_data> anim::convert_curve(QString curve)
{
    QList<key_data> data;

    for (QString frame_string : curve.split('f'))
    {
        if (frame_string.isEmpty())
            continue;

        data.push_back(get_keyframe(frame_string));
    }

    return data;
}

QString anim::update_curve(QString curve, float value, int frame) {}

QString anim::curve_data_to_string(QList<anim::key_data> keys)
{
    QString str_curve;
    for (key_data key : keys)
    {
        float left_angle = key.left_angle;
        float right_angle = key.right_angle;

        QString handler;
        if (left_angle != 0 && right_angle != 0)
            handler = " l" + QString::number(left_angle) + " r" +
                      QString::number(right_angle);

        str_curve += 'f' + QString::number((int)key.frame) + ' ' +
                     QString::number(key.value) + handler + ' ';
    }

    return str_curve;
}

QString anim::curve_to_string(curve *_curve)
{
    QString str_curve;
    for (key_frame *key : _curve->get_keys())
    {
        float left_angle = key->get_left_angle();
        float right_angle = key->get_right_angle();

        QString handler;
        if (left_angle != 0 && right_angle != 0)
            handler = " l" + QString::number(left_angle) + " r" +
                      QString::number(right_angle);

        str_curve += 'f' + QString::number((int)key->x()) + ' ' +
                     QString::number(key->y()) + handler + ' ';
    }

    return str_curve;
}

QString anim::delete_keys_from_curve(QString curve, QList<int> indexs_to_delete)
{
    auto keys_data = convert_curve(curve);
    QList<key_data> new_keys_data;

    for (int i = 0; i < keys_data.count(); i++)
    {
        if (!indexs_to_delete.contains(i))
            new_keys_data.push_back(keys_data.value(i));
    }

    return curve_data_to_string(new_keys_data);
}
