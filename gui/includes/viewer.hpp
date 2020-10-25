#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QGraphicsPixmapItem>
#include <QString>
#include <QComboBox>

#include <qt.hpp>

class graphics_view : public QGraphicsView
{
private:
    void wheelEvent(QWheelEvent *event);

public:
    graphics_view(/* args */);
    ~graphics_view();
};

class viewer : public QWidget
{
private:
    QWidget *player_setup_ui();
    QWidget *timeline_setup_ui();
    QWidget *info_setup_ui();
    QWidget *control_setup_ui();
    QWidget *image_correction_setup_ui();

public:
    viewer(/* args */);
    ~viewer();
};

#endif // VIEWER_H