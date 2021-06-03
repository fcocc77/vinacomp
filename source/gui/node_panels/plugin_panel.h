#ifndef PLUGIN_PANEL_H
#define PLUGIN_PANEL_H

#include <node_panel.h>

class plugin_panel : public node_panel
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
    plugin_panel(QString script_path = "");
    ~plugin_panel();

    inline QString get_script() const override;
    inline void save_script(QString script);
    inline void set_open_script(bool open);
};

inline QString plugin_panel::get_script() const
{
    return script;
}

inline void plugin_panel::save_script(QString _script)
{
    script = _script;
}

inline void plugin_panel::set_open_script(bool open)
{
    open_script = open;
}

#endif // PLUGIN_PANEL_H
