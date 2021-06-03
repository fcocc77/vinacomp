#ifndef FRAME_RANGE_PANEL_H
#define FRAME_RANGE_PANEL_H

#include <node_panel.h>

class frame_range_panel : public node_panel
{
public:
    frame_range_panel();
    ~frame_range_panel();

    void changed(knob *_knob) override;
};

#endif // FRAME_RANGE_PANEL_H
