#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QGraphicsPixmapItem>
#include <QString>

class viewer : public QWidget
{
private:
    /* data */
public:
    viewer(/* args */);
    ~viewer();
};

#endif // VIEWER_H