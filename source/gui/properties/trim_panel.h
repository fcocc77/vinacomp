#ifndef TRIM_PANEL_HPP
#define TRIM_PANEL_HPP

#include <QHBoxLayout>
#include <QJsonArray>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QGraphicsItem>

// Gui
#include <knob.h>
#include <knob_button.h>
#include <nodes_load.h>
#include <properties.h>
#include <tab_widget.h>
#include <node_gui.h>

// Engine
#include <project_struct.h>

class trim_panel : public QWidget
{
private:
    QVBoxLayout *layout;
    QMap<QString, knob *> *knobs;

    QMap<QString, knob_button *> connected_buttons;

    knob_editor *_knob_editor;
    QGraphicsItem *this_node;
    nodes_load *nodes_loaded;
    QPushButton *restart_button;
    QWidget *_vinacomp;
    QList<QWidget *> *viewers_gl;
    
    QStringList tabs_only_read;

    project_struct *project;
    QWidget *_node_view;

    tab_widget *tabs;
    bool is_maximize;
    properties *_properties;

    QLineEdit *name_edit;
    node_gui *_node_gui;
    QWidget *buttons;

    QString name;
    QString type;
    QString icon_name;
    QColor color;

    QJsonObject *data;

    void setup_gui_panels(QJsonArray _knobs, QVBoxLayout *_layout);
    QWidget *top_buttons_setup_ui();
    void setup_knobs(QJsonArray _knobs, QVBoxLayout *layout,
                     QList<QWidget *> *viewers);
    void setup_shared_params();
    void update_render();

public:
    QJsonArray _knobs;

    trim_panel(properties *_properties, QString _name, QString _type,
               QColor color, QString _icon_name, nodes_load *nodes_loaded,
               project_struct *project, QWidget *_vinacomp,
               QGraphicsItem *_node, QWidget *_node_view);
    ~trim_panel();

    void update_controls_knobs(QJsonArray _knobs);
    void maximize(bool _maximize);
    void add_tab(QString tab_name, int index = -1);
    inline QString get_name() const;
    inline QString get_type() const;
    inline QColor get_color() const;
    inline void set_name(QString _name);
    inline knob *get_knob(QString name) const;
    inline bool maximized() const;
    inline QMap<QString, knob *> *get_knobs() const;
    inline tab_widget *get_tab_widget() const;
};

inline tab_widget *trim_panel::get_tab_widget() const
{
    return tabs;
}

inline knob *trim_panel::get_knob(QString name) const
{
    return knobs->value(name);
}

inline QMap<QString, knob *> *trim_panel::get_knobs() const
{
    return knobs;
}

inline QString trim_panel::get_name() const
{
    return name;
}

inline QString trim_panel::get_type() const
{
    return type;
}

inline QColor trim_panel::get_color() const
{
    return color;
}

inline void trim_panel::set_name(QString _name)
{
    name = _name;
    name_edit->setText(name);
}

inline bool trim_panel::maximized() const
{
    return is_maximize;
}

#endif // TRIM_PANEL_HPP
