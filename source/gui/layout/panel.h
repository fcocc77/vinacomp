#ifndef PANEL_HPP
#define PANEL_HPP

// QT5
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QSplitter>
#include <QTabWidget>
#include <QTabBar>

#include <qt.h>
#include <util.h>
#include <node_graph.h>
#include <viewer.h>
#include <script_editor.h>
#include <properties.h>
#include <curve_editor.h>
#include <tab_widget.h>

class panel : public QWidget
{
private:
    QPushButton *setup_cornel_buttons();
    void add_tab(QString name);
    QString get_tab_label(QString name);

    QList<QSplitter *> *splitters;

    QWidget *container;

    node_graph *_node_graph;
    viewer *_viewer;
    script_editor *_script_editor;
    properties *_properties;
    curve_editor *_curve_editor;

    QWidget *panels_layout;

public:
    panel(QWidget *_panels_layout,
          QList<QSplitter *> *_splitters,
          node_graph *_node_graph,
          viewer *_viewer,
          script_editor *_script_editor,
          properties *_properties,
          curve_editor *_curve_editor);
    ~panel();

    QStringList tabs_list;
    tab_widget *tab_section;

    panel *split(Qt::Orientation orientation);
    QSplitter *get_splitter();
    void add_tabs(QStringList tabs_list);
    void close_panel();
    void remove_all_tab();
    void set_index(int index);
};

#endif // PANEL_HPP