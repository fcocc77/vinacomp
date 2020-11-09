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

#include <qt.hpp>
#include <combo_box.hpp>
#include <slider.hpp>
#include <viewer_gl.hpp>
#include <time_line.hpp>

class viewer : public QWidget
{
private:
    QWidget *player_setup_ui();
    QWidget *info_setup_ui();
    QWidget *control_setup_ui();
    QWidget *image_correction_setup_ui();

public:
    viewer(/* args */);
    ~viewer();
};

#endif // VIEWER_H