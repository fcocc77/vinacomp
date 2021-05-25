#ifndef ROTO_GUI_H
#define ROTO_GUI_H

#include <QTreeWidget>

#include <node_gui.h>
#include <button.h>

class roto_gui : public node_gui
{
private:
    QTreeWidget *tree;

    QWidget *buttons;

    button *add_button, *remove_button;
    void setup_knobs(QMap<QString, QVBoxLayout *> layouts) override;

public:
    roto_gui();
    ~roto_gui();

    void changed(knob *_knob) override;
};

#endif // FRAME_RANGE_GUI_H
