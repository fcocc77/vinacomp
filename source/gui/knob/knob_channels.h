#ifndef KNOB_CHANNELS_H
#define KNOB_CHANNELS_H

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>

#include <knob.h>
#include <knob_check_box.h>
#include <global.h>
#include <combo_box.h>
#include <util.h>
#include <global.h>

class knob_channels : public knob
{
    Q_OBJECT
private:
    QHBoxLayout *layout;
    QLineEdit *edit;

    button *accept_button, *cancel_button;
    knob_check_box *red, *green, *blue, *alpha;

    global *glob;
    combo_box *layers;

    void accept();
    void cancel();

    void change_layer(QString layer);
    void visible_layer_edit(bool visible);
    void update_layers();
public:
    knob_channels(global *glob);
    ~knob_channels();

signals:
    void changed(bool value);
};

#endif // KNOB_CHANNELS_H
