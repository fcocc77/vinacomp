#ifndef GROUP_GUI_H
#define GROUP_GUI_H

#include <node_gui.h>
#include <nodes_load.h>

class group_gui : public node_gui
{
private:
    nodes_load *nodes_loaded;
    QString script;

    void export_plugin();
    void edit_script();

public:
    group_gui(nodes_load *nodes_loaded);
    ~group_gui();

    inline QString get_script() const;
    void save_script(QString script);
    void changed(knob *_knob) override;
};

inline QString group_gui::get_script() const
{
    return script;
}


#endif // GROUP_GUI_H
