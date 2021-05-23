#ifndef ROTO_GUI_H
#define ROTO_GUI_H

#include <QTreeWidget>

#include <node_gui.h>
#include <button.h>

class roto_gui : public node_gui
{
private:
    QTreeWidget *tree;
    QVBoxLayout *layout;

    QWidget *buttons;

    button *add_button, *remove_button;
    void setup_knobs() override;

public:
    roto_gui(QVBoxLayout *layout);
    ~roto_gui();

    void changed(knob *_knob) override;
};

#endif // FRAME_RANGE_GUI_H
