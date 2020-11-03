#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

// QT5
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>

#include <trim_panel.hpp>
#include <qt.hpp>

class properties : public QWidget
{
private:
    void setup_ui();
    QWidget *top_buttons_setup_ui();

    QVBoxLayout *trim_panels_layout;

public:
    properties(/* args */);
    ~properties();

    void add_trim_panel(trim_panel *_trim_panel);
};

#endif // PROPERTIES_HPP