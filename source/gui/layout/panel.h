#ifndef PANEL_HPP
#define PANEL_HPP

// QT5
#include <QHBoxLayout>
#include <QMenu>
#include <QPushButton>
#include <QSplitter>
#include <QVBoxLayout>
#include <QWidget>

#include <curve_editor.h>
#include <node_graph.h>
#include <properties.h>
#include <script_editor.h>
#include <tab_widget.h>
#include <viewer.h>

class panel : public QWidget
{
private:
    QList<QSplitter *> *splitters;
    QWidget *container;

    node_graph *_node_graph;
    QLabel *empty_viewer;
    script_editor *_script_editor;
    properties *_properties;
    curve_editor *_curve_editor;
    QWidget *_vinacomp;

    QWidget *_panels_layout;
    tab_widget *_tab_widget;

    QMenu *viewers_menu;
    QMenu *groups_menu;

    QPushButton *setup_cornel_buttons();
    void update_viewers_menu();
    QMap<QString, viewer *> *get_viewers() const;

public:
    panel(QWidget *_panels_layout, QWidget *_vinacomp, QList<QSplitter *> *_splitters,
          node_graph *_node_graph, QLabel *_empty_viewer, script_editor *_script_editor,
          properties *_properties, curve_editor *_curve_editor);
    ~panel();

    void add_fixed_panel(QString name);
    void update_all_viewers_menu();
    void update_all_group_menu();
    panel *split(Qt::Orientation orientation);
    inline void add_tab(QWidget *widget, QString name);
    void add_viewer(viewer *_viewer);
    void remove_viewer(viewer *_viewer);
    void add_group(node_graph *_group);
    void remove_group(node_graph *_group);
    void rename_group(QString old_name, QString new_name);
    void add_tabs(QStringList tabs_list);
    void remove_tab(QString name);
    void close_panel();
    void remove_all_tab();
    void set_index(int index);
    QSplitter *get_splitter() const;
    QStringList get_tabs_list() const;
    inline tab_widget *get_tab_widget() const;
    void update_groups_menu();
};

inline tab_widget *panel::get_tab_widget() const
{
    return _tab_widget;
}

inline void panel::add_tab(QWidget *widget, QString name)
{
    _tab_widget->add_tab(widget, name);
}

#endif // PANEL_HPP
