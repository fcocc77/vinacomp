#ifndef KNOB_EDITOR_H
#define KNOB_EDITOR_H

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>

#include <action.h>
#include <tools.h>

class knob_editor : public QWidget
{
private:
    QVBoxLayout *layout;
    QTreeWidget *knobs_list;
    QWidget *edit_box;

    QLineEdit *knob_name;
    QLineEdit *knob_label;
    QLineEdit *minimum_edit;
    QLineEdit *maximum_edit;
    QLabel *new_line_label;
    QCheckBox *new_line_check;

public:
    knob_editor();
    ~knob_editor();
};

#endif // KNOB_EDITOR_H
