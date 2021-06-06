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
#include <action.h>
#include <tools.h>
#include <tab.h>

struct knob_params
{
    QString name;
    QString label;
    QString tips;
    QString type;
    float min;
    float max;
    float default_value;
    bool over_line;
    bool bidimensional;
    QJsonArray choice_items;
    QJsonArray allowed_file_types;
    bool save_file_dialog;
    bool dir_mode;
    bool image_sequence;
};

class knob_editor : public QWidget
{
private:
    QVBoxLayout *layout;
    QTreeWidget *knobs_list;
    QWidget *edit_box;
    QWidget *_properties;

    QString current_knob_type;
    QString checked_knob_type;

    tools *append_tools;
    QTextEdit *knob_tips;
    QLineEdit *knob_name;
    QLineEdit *knob_label;
    QLineEdit *minimum_edit;
    QLineEdit *maximum_edit;
    QLineEdit *default_value_edit;
    knob_check_box *over_line_check;
    knob_check_box *bidimensional_check;
    knob_check_box *save_file_dialog_check;
    knob_check_box *dir_mode_check;
    knob_check_box *image_sequence_check;
    QTextEdit *allowed_file_types;
    QTextEdit *choice_items_edit;
    QWidget *edit_tools;
    QPushButton *ok_button;
    QPushButton *cancel_button;
    QLabel *edit_label;
    button *edit_icon;

    QList<action *> actions;
    bool insert_knob_or_tab;
    action *add_action;

    bool editing;
    knob *last_knob_under_cursor;
    knob *dragging_knob;
    knob *editing_knob;
    tab *editing_tab;

    int insert_index;
    QWidget *current_panel;

    QString get_available_knob_name(QWidget *panel, QString name) const;
    QString get_available_tab_name(QWidget *panel,
                                   QString preferred_name = "") const;
    QString get_available_name(QStringList name_list, QString name) const;
    QString get_custom_tab_name(QWidget *panel);
    knob_params get_params_from_edit_box(QWidget *panel) const;
    void edit_box_clear();
    void edit_box_close();
    void add_knob(QWidget *panel, knob_params params, int index = -1,
                  int over_line_index = -1);

    void update_knob(knob *_knob, knob_params params, int index,
                     bool keep_name = false, int over_line_index = -1);
    void move_knob(QWidget *panel, int index = -1);
    void push_knob_or_tab();

    void insert_knob_in_tab(QJsonArray *knobs, QJsonObject knob_obj,
                            QString tab_name, int over_line_index = -1,
                            int index = -1);

    QString get_icon_name_from_type(QString knob_type) const;
    QString disable_over_line_to_next_knob(QWidget *panel, QString knob_name);

    QList<QList<QJsonObject>>
    get_knobs_by_tab(QJsonArray knobs, QString tab_name,
                     QJsonArray *leftover_knobs = nullptr) const;

    knob *get_knob_under_cursor();

    QList<knob *> get_line_widget_knobs(QWidget *panel,
                                        QString knob_name) const;

    QVBoxLayout *get_layout_current_tab(QWidget *panel) const;
    int get_index_knob(QWidget *panel, QString knob_name,
                       int *over_line_check = 0) const;
    QWidget *get_panel_from_widget(QWidget *widget) const;
    QWidget *get_panel_under_cursor() const;
    QWidget *get_tab_widget_under_cursor() const;
    void insert_division_to_tabs(QPointF position);
    void insert_division_to_knobs();
    QString add_tab(QWidget *panel, int index = -1, QString preferred_name = "");
    void update_edit_options_from_type(bool visible, QString knob_type);
    void start_insertion();
    void dragging_insertion(QPointF pos = {});
    void hide_all_dividing_line();
    void finish_edit_tab();
    void finish_edit_knob();
    QString get_type_from_knob(knob *_knob) const;

    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public:
    knob_editor(QWidget *properties);
    ~knob_editor();

    void finish_insertion(bool add_item = true);
    void finish_edit(bool ok);
    void edit_tab(tab *_tab);
    void delete_knob(knob *_knob, bool cancel_editing_knob = true);
    void edit_knob(knob *_knob);
    void drag_knob(knob *_knob);
    void drag_knob_move(QWidget *panel);
    void drag_knob_release(QWidget *panel);
};

#endif // KNOB_EDITOR_H
