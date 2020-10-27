#ifndef TRIM_PANEL_HPP
#define TRIM_PANEL_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>

// VinaComp
#include <trim_panel.hpp>
#include <qt.hpp>
#include <knob_color.hpp>
#include <knob_check_box.hpp>
#include <knob_file.hpp>

class trim_panel : public QWidget
{
private:
    QTabWidget *tabs;
    bool is_minimize = false;

    void setup_ui();
    QWidget *top_buttons_setup_ui();
    QTabWidget *tabs_ui();

public:
    trim_panel(/* args */);
    ~trim_panel();
};

#endif //TRIM_PANEL_HPP