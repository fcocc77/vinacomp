#ifndef FRAME_RANGE_GUI_H
#define FRAME_RANGE_GUI_H

#include <knob_intd.h>
#include <node_gui.h>

class frame_range_gui : public node_gui
{
public:
    frame_range_gui();
    ~frame_range_gui();

    void changed( QString param_name ) override;
};

#endif // FRAME_RANGE_GUI_H
