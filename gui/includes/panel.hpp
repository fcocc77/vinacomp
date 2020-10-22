#ifndef PANEL_HPP
#define PANEL_HPP

// QT5
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QSplitter>

#include <qt.hpp>
#include <util.hpp>

class panel : public QWidget
{
private:
    void setup_top_buttons(QWidget *top_buttons);
    void close_panel();

    QList<QSplitter *> *splitters;

    QWidget *container;
    QWidget *tab_section;

public:
    panel(QList<QSplitter *> *_splitters);
    ~panel();

    panel *split(Qt::Orientation orientation);
    QSplitter *get_splitter();
};

#endif // PANEL_HPP