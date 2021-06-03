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
#include <QDockWidget>

// Gui
#include <knob.h>
#include <knob_button.h>
#include <nodes_load.h>
#include <properties.h>
#include <tab_widget.h>
#include <node_panel.h>
#include <knob_curve_menu.h>

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
    knob_curve_menu *curve_menu;

    QStringList tabs_only_read;
    QDockWidget *float_dock;
    bool panel_is_float;

    project_struct *project;
    QWidget *_node_view;

    tab_widget *tabs;
    bool is_maximize;
    properties *_properties;

    QLineEdit *name_edit;
    node_panel *_node_panel;
    QWidget *buttons;

    QString name;
    QString label;
    QString type;
    QString icon_name;
    QColor color;

    action *minimize_action;

    QJsonObject *params;

    QWidget *dividing_line_v, *dividing_line_h;

    void setup_gui_panels();
    QWidget *setup_tool_bar();
    void setup_shared_params();
    void update_render();
    QStringList get_tabs_from_knobs(QJsonArray _knobs);
    void delete_tab(QString tab_name);
    void clean_empty_line_widget();

    void restore_default_values();
    inline void set_label_by_name(QString name);

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public:
    QJsonArray base_knobs;
    QJsonArray shared_knobs;
    QJsonArray *custom_knobs;

    trim_panel(properties *_properties, QString _name, QString _type,
               QColor color, QString _icon_name, nodes_load *nodes_loaded,
               project_struct *project, QWidget *_vinacomp,
               QGraphicsItem *_node, QWidget *_node_view);
    ~trim_panel();

    void float_panel(bool enable, bool relocate = true);
    void update_custom_knobs();
    void maximize(bool _maximize);
    QVBoxLayout *add_tab(QString tab_name, int index = -1);
    void set_edit_mode(bool enable);
    void remove_custom_knob(QString knob_name);
    inline QWidget *get_dividing_line_v() const;
    inline QWidget *get_dividing_line_h() const;
    inline QString get_name() const;
    inline QString get_type() const;
    inline QColor get_color() const;
    void set_name(QString _name);
    inline knob *get_knob(QString name) const;
    inline bool maximized() const;
    inline QMap<QString, knob *> *get_knobs() const;
    inline tab_widget *get_tab_widget() const;
    inline QStringList get_only_read_tabs() const;
    inline QJsonObject *get_params() const;
    inline properties *get_properties() const;
    inline knob_curve_menu *get_curve_menu() const;
    void enter_to_properties();
    void leave_properties(bool disable_edit_mode = true);
    void unlink_node_to_knobs(QString node_name);
    void unlink_all_knobs();
    inline node_panel *get_node_panel() const;
    inline QWidget *get_node_view() const;
};

inline QWidget *trim_panel::get_node_view() const
{
    return _node_view;
}

inline node_panel *trim_panel::get_node_panel() const
{
    return _node_panel;
}

inline knob_curve_menu *trim_panel::get_curve_menu() const
{
    return curve_menu;
}

inline properties *trim_panel::get_properties() const
{
    return _properties;
}

inline QJsonObject *trim_panel::get_params() const
{
    return params;
}

inline QWidget *trim_panel::get_dividing_line_v() const
{
    return dividing_line_v;
}

inline QWidget *trim_panel::get_dividing_line_h() const
{
    return dividing_line_h;
}

inline QStringList trim_panel::get_only_read_tabs() const
{
    return tabs_only_read;
}

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

inline void trim_panel::set_label_by_name(QString name)
{
    label = name.split('.').last();
    name_edit->setText(label);
}

inline bool trim_panel::maximized() const
{
    return is_maximize;
}

#endif // TRIM_PANEL_HPP
