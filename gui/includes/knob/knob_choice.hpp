#ifndef KNOB_CHOICE_HPP
#define KNOB_CHOICE_HPP

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

#include <combo_box.hpp>
#include <knob.hpp>

class knob_choice : public knob
{
private:
    combo_box *choice;

public:
    knob_choice(QStringList _list);
    ~knob_choice();

    void add_items(QStringList list);
};

#endif //KNOB_CHOICE_HPP