#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QGraphicsPixmapItem>
#include <QString>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>

#include <qt.h>
#include <combo_box.h>
#include <slider.h>
#include <viewer_gl.h>
#include <time_line.h>

class viewer : public QWidget
{
private:
    QVBoxLayout *layout;

    viewer_gl *_viewer_gl;
    time_line *_time_line;

    //
    void setup_ui();
    QWidget *player_setup_ui();
    QWidget *info_setup_ui();
    QWidget *control_setup_ui();
    QWidget *image_correction_setup_ui();
    //
    //

public:
    viewer(/* args */);
    ~viewer();
};

#endif // VIEWER_H