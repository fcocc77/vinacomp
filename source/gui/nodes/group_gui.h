#ifndef GROUP_GUI_H
#define GROUP_GUI_H

#include <node_gui.h>
#include <nodes_load.h>

class group_gui : public node_gui
{
private:
    nodes_load *nodes_loaded;
    QString script;
    bool open_script;

    void export_plugin();
    void edit_script();
    void run_script(QString node_name, QString param_name);

    void close() override;
    void setup_knobs(QMap<QString, QVBoxLayout *> layouts) override;

public:
    group_gui(nodes_load *nodes_loaded);
    ~group_gui();

    inline QString get_script() const override;
    void save_script(QString script);
    void changed(knob *_knob) override;
    inline void set_open_script(bool open);
};

inline void group_gui::set_open_script(bool open)
{
    open_script = open;
}

inline QString group_gui::get_script() const
{
    return script;
}


#endif // GROUP_GUI_H
