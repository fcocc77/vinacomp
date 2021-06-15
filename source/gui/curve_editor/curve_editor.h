#ifndef CURVE_EDITOR_HPP
#define CURVE_EDITOR_HPP

// QT5
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>

#include <curve_view.h>
#include <curve_tree.h>
#include <trim_panel.h>

class curve_editor : public QWidget
{
private:
    curve_view *view;
    curve_tree *knobs_tree;
    QWidget *_vinacomp;

    QMap<QString, trim_panel *> panels;

    void update_param(curve *_curve);
    knob *get_knob(QString node_name, QString param_name);
    void update_viewers(int frame);
    void delete_node_item(QString node_name);
    void delete_keyframes(QMap<QString, QList<int>> curves);

    void add_param_item(trim_panel *panel, QString param_name);
    void remove_param_item(trim_panel *panel, QString param_name);

public:
    curve_editor(QWidget *_vinacomp);
    ~curve_editor();

    void update_from_trim_panel(trim_panel *_trim_panel);
    void update_curve(knob *_knob);
    void delete_panel(trim_panel *panel);
    inline curve_view *get_curve_view() const;
};

inline curve_view *curve_editor::get_curve_view() const
{
    return view;
}

#endif // CURVE_EDITOR_HPP
