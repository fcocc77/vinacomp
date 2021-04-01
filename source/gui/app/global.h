#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QList>

struct layer_struct {
    QString name;
    bool red;
    bool green;
    bool blue;
    bool alpha;
};

struct global {
    QList<layer_struct> layers;
};


#endif // GLOBAL_H
