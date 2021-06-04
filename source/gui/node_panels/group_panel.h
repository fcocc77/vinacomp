#ifndef GROUP_PANEL_H
#define GROUP_PANEL_H

#include <node_panel.h>
#include <plugin_panel.h>
#include <nodes_load.h>

class group_panel : public plugin_panel
{
private:
    nodes_load *nodes_loaded;

    bool check_name_and_group_name(QString *plugin_name = 0,
                                   QString *group_name = 0) const;
    void export_plugin_in_py_plugins();
    void export_plugin(QString plugin_path);
    void export_to_another_dir();
    void edit_script();

    void close() override;
    QJsonObject get_child_nodes() const;
    QJsonObject get_params() const;

public:
    group_panel(nodes_load *nodes_loaded);
    ~group_panel();

    void changed(knob *_knob) override;
};

#endif // GROUP_PANEL_H
