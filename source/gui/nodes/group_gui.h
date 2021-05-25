#ifndef GROUP_GUI_H
#define GROUP_GUI_H

#include <node_gui.h>
#include <node_plugin_gui.h>
#include <nodes_load.h>

class group_gui : public node_plugin_gui
{
private:
    nodes_load *nodes_loaded;

    void export_plugin();
    void edit_script();

    void close() override;

public:
    group_gui(nodes_load *nodes_loaded);
    ~group_gui();

    void changed(knob *_knob) override;
};

#endif // GROUP_GUI_H
