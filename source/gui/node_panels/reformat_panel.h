#ifndef REFORMAT_PANEL_H
#define REFORMAT_PANEL_H

#include <node_panel.h>

class reformat_panel : public node_panel
{
public:
    reformat_panel();
    ~reformat_panel();

    void changed(knob *_knob) override;
};

#endif // REFORMAT_PANEL_H
