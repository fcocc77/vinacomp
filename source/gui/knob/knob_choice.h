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
    int default_index;

    void restore_default() override;
    void restore_param() override;

public:
    knob_choice(knob_props props, QList<combo_box_item> items = {},
                int default_index = 0);
    ~knob_choice();

    inline int get_index() const;
    inline void set_index(int index);
    inline QVariant get_value() const;

signals:
    void changed(QVariant value, int index);
};

inline void knob_choice::set_index(int index)
{
    choice->set_index(index, false);
}

inline int knob_choice::get_index() const
{
    return choice->get_index();
}

inline QVariant knob_choice::get_value() const
{
    return choice->get_value();
}

#endif // KNOB_CHOICE_HPP
