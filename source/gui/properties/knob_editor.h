#ifndef KNOB_EDITOR_H
#define KNOB_EDITOR_H

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QJsonArray>
#include <QMouseEvent>

#include <knob.h>

class knob_editor : public QWidget
{
private:
    QVBoxLayout *layout;
    QTreeWidget *knobs_list;
    QWidget *edit_box;
    QWidget *_properties;
    QString current_knob_type;

    QLineEdit *knob_name;
    QLineEdit *knob_label;
    QLineEdit *minimum_edit;
    QLineEdit *maximum_edit;
    QLabel *new_line_label;
    QCheckBox *new_line_check;

    QWidget *temp_widget;

    // edit
    int insert_index;
    QWidget *current_panel;

    QString get_available_name(QWidget *panel) const;
    void add_knob(QWidget *panel, int index = -1);
    void push_knob();
    knob *get_knob_under_cursor() const;
    QVBoxLayout *get_controls_layout(QWidget *panel) const;
    int get_index_knob(QWidget *panel, QString knob_name) const;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public:
    knob_editor(QWidget *properties);
    ~knob_editor();
};

#endif // KNOB_EDITOR_H
