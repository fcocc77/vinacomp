#ifndef GROUP_GUI_H
#define GROUP_GUI_H

#include <node_gui.h>

class group_gui : public node_gui
{
private:
    void export_plugin();

public:
    group_gui();
    ~group_gui();

    void changed(knob *_knob) override;
};

#endif // GROUP_GUI_H
