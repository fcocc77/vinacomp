#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

// QT5
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>

#include <qt.hpp>

class properties : public QWidget
{
private:
    void setup_ui();
    QWidget *top_buttons_setup_ui();

    QVBoxLayout *trim_panels_layout;
    QWidget *trim_panels;

public:
    properties(/* args */);
    ~properties();

    template <class T>
    void add_trim_panel(T *_trim_panel);

    template <class T>
    void close_trim_panel(T *_trim_panel);
};
//
//

// Implementacion de Plantillas
template <class T>
void properties::add_trim_panel(T *_trim_panel)
{
    this->hide();
    _trim_panel->show();
    trim_panels_layout->addWidget(_trim_panel);
    this->show();
}

template <class T>
void properties::close_trim_panel(T *_trim_panel)
{
    _trim_panel->setParent(0);
    _trim_panel->hide();
}

#endif // PROPERTIES_HPP