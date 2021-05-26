#ifndef NODE_PLUGIN_GUI_H
#define NODE_PLUGIN_GUI_H

#include <node_gui.h>

class node_plugin_gui : public node_gui
{
private:
    void convert_to_group();

protected:
    QString script;
    bool open_script;
    bool is_plugin;

    void run_script(QString node_name, QString param_name);
    void changed(knob *_knob) override;
    void setup_knobs(QMap<QString, QVBoxLayout *> layouts) override;

public:
    node_plugin_gui(QString script_path = "");
    ~node_plugin_gui();

    inline QString get_script() const override;
    inline void save_script(QString script);
    inline void set_open_script(bool open);
};

inline QString node_plugin_gui::get_script() const
{
    return script;
}

inline void node_plugin_gui::save_script(QString _script)
{
    script = _script;
}

inline void node_plugin_gui::set_open_script(bool open)
{
    open_script = open;
}

#endif // NODE_PLUGIN_GUI_H
