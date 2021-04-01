#ifndef NEW_LAYERS_H
#define NEW_LAYERS_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>

#include <button.h>
#include <knob_check_box.h>
#include <global.h>
#include <combo_box.h>

class new_layers : public QWidget
{
private:
    QHBoxLayout *layout;
    QLineEdit *edit;

    button *accept_button, *cancel_button;
    knob_check_box *red, *green, *blue, *alpha;

    global *glob;
    combo_box *layers;

    void accept();
    void cancel();

public:
    new_layers(global *glob, combo_box *layers);
    ~new_layers();

    void add_to_combo_box(combo_box *combo);
};

#endif // NEW_LAYERS_H
