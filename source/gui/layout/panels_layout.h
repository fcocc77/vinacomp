#ifndef PANELS_LAYOUT_HPP
#define PANELS_LAYOUT_HPP

#include <QWidget>
#include <QApplication>

#include <panel.h>
#include <node_graph.h>
#include <viewer.h>
#include <script_editor.h>
#include <properties.h>
#include <curve_editor.h>

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

    void save_json_layout(QSplitter *splitter, int deep = 0, QString letter = "", QStringList parents = {});
    void load_splitter(QJsonObject splitter_obj, panel *panel_a);
    QJsonObject json_layout;

    bool isolate;
    panel *first_panel;

public:
    panels_layout(
		QWidget *_vinacomp,
        node_graph *_node_graph,
        QLabel *empty_viewer,
        script_editor *_script_editor,
        properties *_properties,
        curve_editor *_curve_editor);
    ~panels_layout();

    QList<QSplitter *> *splitters;

    QAction *restore_default_action;
    QAction *save_current_action;

	panel *get_viewer_panel() const;
	QList <panel *> get_all_panels() const;
};

#endif // PANELS_LAYOUT_HPP
