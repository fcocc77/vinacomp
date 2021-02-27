#ifndef KNOB_INTD_H
#define KNOB_INTD_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>

#include <slider.h>
#include <qt.h>
#include <knob.h>

class knob_intd : public knob
{
	Q_OBJECT
private:
	QList < QLineEdit * > dimensions_edits;

public:
    knob_intd(QList <int> default_values = {0, 1});
    ~knob_intd();

	int get_value(int dimension) const;
	QList <int> get_values() const;
	void set_value(int value, int dimension = 0);
	void set_value(QList <int> values);
signals:
	void changed(QList <int> values);
};

#endif // KNOB_INTD_H
