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

    QMap<QString, QWidget *> *panels;

    QVBoxLayout *trim_panels_layout;
    QWidget *trim_panels;

    bool is_maximize = true;
    int max_panels = 10;

    void minimize_all_panels();
    void close_all();
    void limit_panels(int amount);

public:
    properties(/* args */);
    ~properties();

    void close_trim_panel(QString panel_name);

    template <class T>
    void add_trim_panel(T *_trim_panel);
};
//
//

// Implementacion de Plantillas
template <class T>
void properties::add_trim_panel(T *_trim_panel)
{
    limit_panels(max_panels - 1);

    panels->insert(_trim_panel->get_name(), _trim_panel);
    _trim_panel->maximize(true);

    this->hide();
    _trim_panel->show();
    trim_panels_layout->addWidget(_trim_panel);
    this->show();
}

#endif // PROPERTIES_HPP