#ifndef ROTO_GUI_H
#define ROTO_GUI_H

#include <node_gui.h>

class roto_gui : public node_gui
{
public:
    roto_gui();
    ~roto_gui();

    void changed(knob *_knob) override;
};

#endif // FRAME_RANGE_GUI_H
