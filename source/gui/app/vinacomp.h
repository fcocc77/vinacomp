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
#include <settings.h>
#include <action.h>
#include <project_settings.h>

class vinacomp : public QMainWindow
{
    Q_OBJECT
private:
    node_graph *_node_graph;
    viewer *_viewer;
    script_editor *_script_editor;
    properties *_properties;
    curve_editor *_curve_editor;
    tools *_tool_bar;
    settings *_settings;
	project_settings *_project_settings;

    panels_layout *_panels_layout;
    bool fullscreen;
	bool project_settings_visible;

	// Acciones
    QAction *update_sylesheet_action;
	action *new_project_action;
	action *open_project_action;
	action *settings_action;
	action *project_settings_action;
	action *script_layout_action;
	action *comp_layout_action;
	//

    // en este json estara todo el proyecto actual, aqui se guardara todo,
    // para guardarlo en un archivo o enviarlo al motor de render.
    bool project_opened;
    QString current_project;
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
	void tool_bar();
    void save_as();
    void to_save_project();
    void save_project(QString project_path);

public:
    vinacomp();
    ~vinacomp();
};

#endif // VINACOMP_H
