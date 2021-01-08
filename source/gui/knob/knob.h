#ifndef KNOB_HPP
#define KNOB_HPP

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class knob : public QWidget
{
private:
    /* data */
public:
    knob(/* args */);
    ~knob();

    void set_init_space(int space, QString label = "");

    QLabel *label_widget;
    QWidget *init_space;

	int icon_size;
};

#endif //KNOB_HPP
