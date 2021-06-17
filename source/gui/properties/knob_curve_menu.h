#ifndef KNOB_CURVE_MENU_H
#define KNOB_CURVE_MENU_H

#include <QMenu>
#include <knob.h>

class knob_curve_menu : public QMenu
{
private:
    knob *current_knob;
    int current_dimension;
    QWidget *panel;

    void set_key();
    void no_animation();
    void edit_expression();
    void clear_expression();
    void reset_default();

    void copy(QString copy_action);
    void paste();
    void remove_link();

public:
    knob_curve_menu(QWidget *panel);
    ~knob_curve_menu();

    inline void set_knob(knob *_knob, int dimension = -1);
};

inline void knob_curve_menu::set_knob(knob *_knob, int dimension)
{
    current_knob = _knob;
    current_dimension = dimension;
}

#endif // KNOB_CURVE_MENU_H
