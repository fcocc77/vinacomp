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
    void show_curve(QString node_name, QString param_name, QString curve);
    knob *get_knob(QString node_name, QString param_name);
    void update_viewers(int frame);
    void delete_node_item(QString node_name);

public:
    curve_editor(QWidget *_vinacomp);
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
