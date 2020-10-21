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
    void split(Qt::Orientation orientation);
    void close_panel();

    QWidget *container;
    QWidget *tab_section;

public:
    panel();
    ~panel();
};

#endif // PANEL_HPP