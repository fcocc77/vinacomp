#ifndef PROJECT_STRUCT_H
#define PROJECT_STRUCT_H

#include <QColor>
#include <QJsonObject>
#include <QList>
#include <QMap>
#include <QPointF>
#include <QRectF>

struct node_struct
{
    QString name;
    QColor color;
    QString type;
    QString tips;
    QPointF pos;
    QJsonObject *params;
    QJsonObject inputs;

    // estos 2 atributos son solo para el backdrop
    QSize size;
    int z_value;
};

struct layer_struct {
    QString name;
    bool red;
    bool green;
    bool blue;
    bool alpha;
};

struct global_struct
{
    QList<layer_struct> layers;
    QRectF node_view_rect;
};

class project_struct
{
public:
    project_struct();
    ~project_struct();

    QMap<QString, node_struct> nodes;
    global_struct global;
    int frame;

    void insert_node(QString _name, QString _type, QJsonObject _params = {},
                     QColor _color = QColor(), QPointF _pos = {0, 0},
                     QJsonObject _inputs = {}, QSize size = {0, 0},
                     int z_value = 0, QString tips = "");

    void delete_node(QString name);

    void insert_input(QString src, QString dst, int index);
    void delete_input(QString node, int index);

    QJsonObject get_project_json() const;

    void save(QString project_path) const;
    void load(QString project_path);

    static QJsonValue get_value_frame(QJsonObject *params, QString param_name,
                                      int frame);
};

#endif // PROJECT_STRUCT_H
