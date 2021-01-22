#ifndef KNOB_CHOICE_HPP
#define KNOB_CHOICE_HPP

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

#include <combo_box.h>
#include <knob.h>

class knob_choice : public knob
{
	Q_OBJECT
private:
    combo_box *choice;

public:
    knob_choice(QStringList items, int default_index);
    ~knob_choice();

	int get_index() const;
	QString get_value() const;

signals:
	void changed(QString name, int index);
};

#endif //KNOB_CHOICE_HPP
