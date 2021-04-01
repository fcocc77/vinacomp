#ifndef NEW_LAYERS_H
#define NEW_LAYERS_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>

#include <button.h>
#include <knob_check_box.h>
#include <global.h>

class new_layers : public QWidget
{
private:
    QHBoxLayout *layout;
    QLineEdit *edit;

    button *accept_button, *cancel_button;
    knob_check_box *red, *green, *blue, *alpha;

    global *glob;

    void accept();
    void cancel();

public:
    new_layers(global *glob);
    ~new_layers();
};

#endif // NEW_LAYERS_H
