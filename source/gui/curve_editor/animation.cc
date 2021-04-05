#include <animation.h>
#include <util.h>

int anim::get_value(QString curve, int frame)
{
    // Extraer el valor del frame, de la curva
    curve = "f134 20 l0 r0 f500 20.9 f1043 5";

    QString _frame = QString::number(frame);

    QString captured;
    bool capture = false;
    for (QChar c : curve)
    {
        if (capture)
            captured += c;
        else
        {
            print(captured);
        }

        if (c == 'f')
            capture = true;
        if (c == ' ')
            capture = false;
    }

    return 10;
}

float anim::get_valuef(QString curve, int frame) {}

QList<anim::key_data> anim::convert_curve(QString curve)
{
    QList<key_data> data;

    for (QString frame_data : curve.split('f'))
    {
        if (frame_data.isEmpty())
            continue;

        QStringList fdata = frame_data.split(' ');
        key_data key;

        key.frame = fdata[0].toInt();
        key.value = fdata[1].toFloat();

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

        data.push_back(key);
    }

    return data;
}

QString anim::update_curve(QString curve, int value, int frame) {}

QString anim::update_curvef(QString curve, int value, float frame) {}
