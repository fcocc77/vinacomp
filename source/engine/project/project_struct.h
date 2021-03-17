#ifndef PROJECT_STRUCT_H
#define PROJECT_STRUCT_H

#include <QColor>
#include <QJsonObject>
#include <QList>
#include <QMap>
#include <QPointF>

#include <util.h>

struct node_struct
{
    QColor color;
    QString type;
    QPointF pos;
    QJsonObject *params;
    QJsonObject inputs;
};

class project_struct
{
public:
    project_struct();
    ~project_struct();

    QMap<QString, node_struct> nodes;

    void insert_node( QString _name, QColor _color, QString _type, QPointF _pos,
                      QJsonObject _params = {}, QJsonObject _inputs = {} );
    void delete_node( QString name );

    void insert_input( QString src, QString dst, int index );
    void delete_input( QString node, int index );

    QJsonObject get_project_json() const;

    void save( QString project_path ) const;
    void load( QString project_path );
};

#endif // PROJECT_STRUCT_H
