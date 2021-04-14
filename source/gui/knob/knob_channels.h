#ifndef KNOB_CHANNELS_H
#define KNOB_CHANNELS_H

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>

#include <combo_box.h>
#include <project_struct.h>
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

    QString current_layer;

    combo_box *layers;
    QString action;
    QString layer_to_edit;
    int layer_to_edit_index;

    void to_emmit_signal();
    QString checking();
    void add_layer();
    void edit_layer();
    void visible_layer_edit(bool visible);
    void update_layers(QString from = "", bool keep_currnet_layer = false);
    void set_all_channel(bool value);
    void set_layer(QString layer);

    void restore_param() override;

    layer_struct *get_layer(QString layer);

public:
    knob_channels(project_struct *project = nullptr, QString layer = "main",
                  QList<bool> channels = {true, true, true, true});
    ~knob_channels();

signals:
    void changed(QString layer, QList<bool> channels);
};

#endif // KNOB_CHANNELS_H
