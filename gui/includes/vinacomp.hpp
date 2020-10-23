#ifndef VINACOMP_H
#define VINACOMP_H

// QT5
#include <QMainWindow>
#include <QSplitter>
#include <QMenuBar>

// VINA COMP
#include <viewer.hpp>
#include <node_graph.hpp>
#include <script_editor.hpp>
#include <properties.hpp>
#include <curve_editor.hpp>
#include <util.hpp>
#include <panels_layout.hpp>

class vinacomp : public QMainWindow
{
    Q_OBJECT
private:
    node_graph *_node_graph;
    viewer *_viewer;
    script_editor *_script_editor;
    properties *_properties;
    curve_editor *_curve_editor;

    panels_layout *_layout;

    QAction *update_sylesheet_action;

public:
    vinacomp(QWidget *parent = 0);
    ~vinacomp();

    void setup_ui();
    void setup_style();
    void main_menu();
};

#endif // VINACOMP_H