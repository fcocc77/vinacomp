#ifndef CURVE_EDITOR_HPP
#define CURVE_EDITOR_HPP

// QT5
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTreeWidget>
#include <QWidget>

#include <curve_view.h>
#include <trim_panel.h>
#include <project_struct.h>

class curve_editor : public QWidget
{
private:
    curve_view *view;
    QTreeWidget *tree;

    QMap<QString, trim_panel *> panels;

    QTreeWidget *knobs_tree_setup_ui();
    void add_item(QString node, QString param, QString dimension);
    QTreeWidgetItem *get_node_item(QString item_name) const;
    QTreeWidgetItem *get_param_item(QTreeWidgetItem *node_item,
                                    QString param_name) const;

    void update_param(curve *_curve);
    void delete_node_item(QString node_name);
    void show_curve(QString node_name, QString param_name, QString curve);
    knob *get_knob(QString node_name, QString param_name);

public:
    curve_editor(project_struct *project);
    ~curve_editor();

    void update_from_trim_panel(trim_panel *_trim_panel);
    void delete_curve(QString node_name);
    inline curve_view *get_curve_view() const;
};

inline curve_view *curve_editor::get_curve_view() const
{
    return view;
}

#endif // CURVE_EDITOR_HPP
