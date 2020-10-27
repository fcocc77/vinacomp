#ifndef TRIM_PANEL_HPP
#define TRIM_PANEL_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>

#include <trim_panel.hpp>
#include <qt.hpp>

class trim_panel : public QWidget
{
private:
    void setup_ui();
    QWidget *top_buttons_setup_ui();
    QTabWidget *tabs_ui();

public:
    trim_panel(/* args */);
    ~trim_panel();
};

#endif //TRIM_PANEL_HPP