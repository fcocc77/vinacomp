#ifndef SPLITTER_H
#define SPLITTER_H

// QT5
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QSplitter>

#include <qt.hpp>
#include <util.hpp>

class splitter : public QWidget
{
private:
    void setup_top_buttons(QWidget *top_buttons);

    void horizontal_split(QWidget *parent);
    void vertical_split(QWidget *parent);


    QWidget *tab_section;

public:
    splitter(/* args */);
    ~splitter();
};

#endif // SPLITTER_H