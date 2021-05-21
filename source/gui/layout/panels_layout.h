#ifndef PANELS_LAYOUT_HPP
#define PANELS_LAYOUT_HPP

#include <QApplication>
#include <QWidget>

#include <curve_editor.h>
#include <node_graph.h>
#include <panel.h>
#include <properties.h>
#include <script_editor.h>
#include <viewer.h>

class panels_layout : public QWidget
{
private:
    void save_layout();
    void load_layout();
    void restore_default();
    void isolate_panel();
    void set_visible_panels(bool visible);

    panel *get_child_panel(QSplitter *__splitter, QString _letter) const;
    panel *get_panel_from_cursor() const;

    void save_json_layout(QSplitter *splitter, int deep = 0, QString letter = "",
                          QStringList parents = {});
    void load_splitter(QJsonObject splitter_obj, panel *panel_a);
    QJsonObject json_layout;

    panel *get_some_empty_panel() const;
    panel *get_panel(QString name) const;

    bool isolate;
    panel *first_panel;
    QWidget *_vinacomp;

public:
    panels_layout(QWidget *_vinacomp, node_graph *_node_graph, QLabel *empty_viewer,
                  script_editor *_script_editor, properties *_properties,
                  curve_editor *_curve_editor);
    ~panels_layout();

    QList<QSplitter *> *splitters;

    QAction *restore_default_action;
    QAction *save_current_action;

    QList<panel *> get_all_panels() const;
    void add_viewer(viewer *_viewer);
    void delete_viewer(viewer *_viewer);
    void add_node_graph_group(node_graph *group, QString name);
    void rename_node_graph_group(QString old_name, QString new_name);
    void delete_node_graph_group(node_graph *group);
};

#endif // PANELS_LAYOUT_HPP
