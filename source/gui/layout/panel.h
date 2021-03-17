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
#include <qt.h>
#include <script_editor.h>
#include <tab_widget.h>
#include <util.h>
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
    QStringList tabs_list;
    tab_widget *_tab_widget;

    QMenu *viewers_menu;

    QPushButton *setup_cornel_buttons();
    QString get_tab_label( QString name );
    void add_fixed_panel( QString name );
    void update_viewers_menu();
    QList<viewer *> *get_viewers() const;

public:
    panel( QWidget *_panels_layout, QWidget *_vinacomp, QList<QSplitter *> *_splitters,
           node_graph *_node_graph, QLabel *_empty_viewer, script_editor *_script_editor,
           properties *_properties, curve_editor *_curve_editor );
    ~panel();

    panel *split( Qt::Orientation orientation );
    void add_tab( QWidget *widget, QString name );
    void add_viewer( viewer *_viewer );
    void add_tabs( QStringList tabs_list );
    void close_panel();
    void remove_all_tab();
    void set_index( int index );
    QSplitter *get_splitter() const;
    QStringList get_tabs_list() const;
    tab_widget *get_tab_widget() const;
};

#endif // PANEL_HPP
