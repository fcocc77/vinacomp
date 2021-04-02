#ifndef KNOB_CHANNELS_H
#define KNOB_CHANNELS_H

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>

#include <combo_box.h>
#include <global.h>
#include <knob.h>
#include <knob_check_box.h>
#include <qt.h>
#include <util.h>

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

    void add_layer();
    void visible_layer_edit(bool visible);
    void update_layers(bool from_add_layer = false);
    void set_all_channel(bool value);

public:
    knob_channels(global *glob);
    ~knob_channels();

signals:
    void changed(QString layer, int index);
};

#endif // KNOB_CHANNELS_H
