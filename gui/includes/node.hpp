#ifndef NODE_H
#define NODE_H

#include <QWidget>
#include <QHBoxLayout>
#include <QGraphicsRectItem>
#include <QPen>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QPainter>
#include <QFocusEvent>

#include <util.hpp>

class node : public QGraphicsPathItem
{
private:
    QGraphicsScene *scene;
    QMap<QString, node *> *selected_nodes;

    QPoint *center_position;

    int minimum_width;
    int minimum_height;
    int current_width;
    int current_height;

    int *current_z_value;

    int icon_area_width;
    QGraphicsTextItem *name;
    QGraphicsTextItem *tips;

    void change_size_rectangle(int _width, int _height);

public:
    node(QGraphicsScene *_scene,
         QMap<QString, node *> *_selected_nodes,
         int *_current_z_value);
    ~node();

    void set_name(QString name);
    void set_tips(QString tips);
    QString get_name();
    void set_position(int x, int y);
    QPoint get_center_position();
    QList<int> get_size();
    void set_icon(QString icon_name);
    void set_selected_style(bool enable);

};

#endif // NODE_H