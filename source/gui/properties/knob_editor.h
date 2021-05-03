#ifndef KNOB_EDITOR_H
#define KNOB_EDITOR_H

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QJsonArray>

class knob_editor : public QWidget
{
private:
    QVBoxLayout *layout;
    QTreeWidget *knobs_list;
    QWidget *edit_box;
    QWidget *panel;
    QString current_knob_type;

    QLineEdit *knob_name;
    QLineEdit *knob_label;
    QLineEdit *minimum_edit;
    QLineEdit *maximum_edit;
    QLabel *new_line_label;
    QCheckBox *new_line_check;

    // edit
    QJsonArray knobs;
    QString get_available_name() const;
    void add_knob();

public:
    knob_editor(QWidget *panel);
    ~knob_editor();
};

#endif // KNOB_EDITOR_H
