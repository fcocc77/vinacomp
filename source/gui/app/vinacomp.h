#ifndef VINACOMP_H
#define VINACOMP_H

// QT5
#include <QApplication>
#include <QFileDialog>
#include <QMainWindow>
#include <QMenuBar>
#include <QSplitter>

// Gui
#include <action.h>
#include <curve_editor.h>
#include <node_graph.h>
#include <panels_layout.h>
#include <project_settings.h>
#include <properties.h>
#include <script_editor.h>
#include <general_settings.h>
#include <viewer.h>
#include <tools.h>
#include <file_dialog.h>

// Engine
#include <project_struct.h>
#include <renderer.h>

class vinacomp : public QMainWindow
{
    Q_OBJECT
private:
    node_graph *_node_graph;
    script_editor *_script_editor;
    properties *_properties;
    curve_editor *_curve_editor;
    tools *_tool_bar;
    general_settings *_settings;
    project_settings *_project_settings;
    renderer *_renderer;
    file_dialog *_file_dialog;

    QMap<QString, viewer *> *viewers;
    QList<QWidget *> *viewers_gl;
    QMap<QString, node_graph*> *groups_node_graph;

    panels_layout *_panels_layout;
    bool fullscreen;
    bool project_settings_visible;
    bool settings_visible;

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
    project_struct *project;
    QJsonObject *project_old;
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

    // undo & redo
    QList<QJsonObject> history;
    int current_history;

    void restore_history(QJsonObject record);
    void undo();
    void redo();
    //

public:
    vinacomp();
    ~vinacomp();

    // undo & redo
    void add_history();
    //

    inline panels_layout *get_panels_layout() const;
    inline QMap<QString, viewer *> *get_viewers() const;
    inline QList<QWidget *> *get_viewers_gl() const;
    inline project_struct *get_project() const;
    inline renderer *get_renderer() const;
    void update_render_all_viewer(bool clear_init_image = false) const;
    inline project_settings *get_project_settings() const;
    inline properties *get_properties() const;
    inline curve_editor *get_curve_editor() const;
    inline QMap<QString, node_graph*> *get_groups_node_graph() const;
    inline file_dialog *get_file_dialog() const;
};

inline file_dialog *vinacomp::get_file_dialog() const
{
    return _file_dialog;
}

inline curve_editor *vinacomp::get_curve_editor() const
{
    return _curve_editor;
}

inline panels_layout *vinacomp::get_panels_layout() const
{
    return _panels_layout;
}

inline QMap<QString, node_graph *> *vinacomp::get_groups_node_graph() const
{
    return groups_node_graph;
}

inline QMap<QString, viewer *> *vinacomp::get_viewers() const
{
    return viewers;
}

inline QList<QWidget *> *vinacomp::get_viewers_gl() const
{
    viewers_gl->clear();
    for (viewer *_viewer : *viewers)
        viewers_gl->push_back(_viewer->get_viewer_gl());

    return viewers_gl;
}

inline project_struct *vinacomp::get_project() const
{
    return project;
}

inline renderer *vinacomp::get_renderer() const
{
    return _renderer;
}

inline project_settings *vinacomp::get_project_settings() const
{
    return _project_settings;
}

inline properties *vinacomp::get_properties() const
{
    return _properties;
}

#endif // VINACOMP_H
