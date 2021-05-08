#ifndef KNOB_CHOICE_HPP
#define KNOB_CHOICE_HPP

#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

#include <combo_box.h>
#include <knob.h>

class knob_choice : public knob
{
    Q_OBJECT
private:
    combo_box *choice;
    void restore_param() override;

public:
    knob_choice(knob_props props, QList<combo_box_item> items = {},
                int default_index = 0);
    ~knob_choice();

    int get_index() const;
    QVariant get_value() const;

signals:
    void changed(QVariant value, int index);
};

#endif // KNOB_CHOICE_HPP
