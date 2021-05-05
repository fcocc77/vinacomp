#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

// QT5
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

#include <knob_editor.h>

class properties : public QWidget
{
private:
    void setup_ui();
    QWidget *top_buttons_setup_ui();

    QVBoxLayout *trim_panels_layout;
    QWidget *trim_panels;
    QWidget *_vinacomp;
    
    knob_editor *_knob_editor;

    bool is_maximize = true;
    int max_panels = 10;

    void minimize_all_panels();
    void close_all();
    void limit_panels(int amount);
    void update_viewers_handlers();
    void update_curve(QWidget *_trim_panel);

public:
    properties(QWidget *_vinacomp);
    ~properties();

    void close_trim_panel(QString panel_name);
    QList<QWidget *> get_trim_panels();
    QWidget *get_trim_panel(QString panel_name);
    QWidget *get_trim_panel(int index);
    QWidget *get_first_panel() ;

    template <class T> void add_trim_panel(T *_trim_panel);

    void update_animated_knobs();
    void set_edit_mode(bool enable);
};
//
//

// Implementacion de Plantillas
template <class T> void properties::add_trim_panel(T *_trim_panel)
{
    limit_panels(max_panels - 1);

    trim_panels_layout->addWidget(_trim_panel);
    _trim_panel->maximize(true);
    _trim_panel->show();

    update_curve(_trim_panel);
    update_viewers_handlers();
}

#endif // PROPERTIES_HPP
