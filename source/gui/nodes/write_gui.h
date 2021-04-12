#ifndef WRITE_GUI_H
#define WRITE_GUI_H

#include <node_gui.h>

class write_gui : public node_gui
{
private:
    void changed(QString param_name) override;

public:
    write_gui();
    ~write_gui();
};

#endif // WRITE_GUI_H
