#ifndef REFORMAT_GUI_H
#define REFORMAT_GUI_H

#include <node_gui.h>

class reformat_gui : public node_gui
{
public:
    reformat_gui();
    ~reformat_gui();

    void changed(QString param_name) override;
};

#endif // REFORMAT_GUI_H
