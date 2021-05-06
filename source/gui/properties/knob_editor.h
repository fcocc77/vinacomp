#ifndef KNOB_EDITOR_H
#define KNOB_EDITOR_H

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QJsonArray>
#include <QMouseEvent>

#include <knob.h>
#include <knob_check_box.h>

class knob_editor : public QWidget
{
private:
    QVBoxLayout *layout;
    QTreeWidget *knobs_list;
    QWidget *edit_box;
    QWidget *_properties;
    QString current_knob_type;

    QTextEdit *knob_tips;
    QLineEdit *knob_name;
    QLineEdit *minimum_edit;
    QLineEdit *maximum_edit;
    knob_check_box *new_line_check;

    QWidget *temp_widget;
    QWidget *temp_vertical_widget;

    // edit
    int insert_index;
    QWidget *current_panel;

    QString get_available_knob_name(QWidget *panel) const;
    QString get_available_tab_name(QWidget *panel,
                                   QString preferred_name = "") const;
    QString get_available_name(QStringList name_list, QString name) const;
    QString get_custom_tab_name(QWidget *panel);
    void add_knob(QWidget *panel, int index = -1);
    void push_knob_or_tab();
    knob *get_knob_under_cursor() const;
    QVBoxLayout *get_controls_layout(QWidget *panel) const;
    int get_index_knob(QWidget *panel, QString knob_name) const;
    QWidget *get_panel_from_widget(QWidget *widget) const;
    QWidget *get_panel_under_cursor() const;
    QWidget *get_tab_widget_under_cursor() const;
    void insert_division_to_tabs(QPointF position);
    void insert_division_to_knobs();
    QString add_tab(QWidget *panel, int index = -1, QString preferred_name = "");
    void update_edit_options(bool visible);

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public:
    knob_editor(QWidget *properties);
    ~knob_editor();
};

#endif // KNOB_EDITOR_H
