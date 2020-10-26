#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QGraphicsPixmapItem>
#include <QString>
#include <QComboBox>
#include <graphics_view.hpp>
#include <QPushButton>
#include <QLineEdit>

#include <qt.hpp>

class viewer_graphics_view : public graphics_view
{
private:

public:
    viewer_graphics_view(/* args */);
    ~viewer_graphics_view();
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