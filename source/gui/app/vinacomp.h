#ifndef VINACOMP_H
#define VINACOMP_H

// QT5
#include <QMainWindow>
#include <QSplitter>
#include <QMenuBar>
#include <QApplication>
#include <QFileDialog>

// VINA COMP
#include <viewer.h>
#include <node_graph.h>
#include <script_editor.h>
#include <properties.h>
#include <curve_editor.h>
#include <util.h>
#include <panels_layout.h>
#include <tool_bar.h>
#include <settings.h>
#include <maker.h>

class vinacomp : public QMainWindow
{
    Q_OBJECT
private:
    node_graph *_node_graph;
    viewer *_viewer;
    script_editor *_script_editor;
    properties *_properties;
    curve_editor *_curve_editor;
    tool_bar *_tool_bar;
    settings *_settings;
    maker *_maker;

    QApplication *app;

    panels_layout *_panels_layout;
    bool fullscreen = false;

    QAction *update_sylesheet_action;

    // en este json estara todo el proyecto actual, aqui se guardara todo,
    // para guardarlo en un archivo o enviarlo al motor de render.
    bool project_opened = false;
    QString current_project = "";
    QJsonObject *project;
    //
    //

    QMenu *recent_projects_menu;
    QJsonArray get_recent_projects();
    void update_recent_projects();
    void recorder_recent_projects(QString project_path);
    void open_project(QString project_path);
    void open_project_dialog();
    void setup_ui();
    void setup_style();
    void main_menu();
    void save_as();
    void to_save_project();
    void save_project(QString project_path);

public:
    vinacomp(QApplication *_app);
    ~vinacomp();
};

#endif // VINACOMP_H