#ifndef KNOB_FLOATD_H
#define KNOB_FLOATD_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>

#include <slider.h>
#include <qt.h>
#include <knob.h>

class knob_floatd : public knob
{
	Q_OBJECT
private:
	QList < QLineEdit * > dimensions_edits;
	bool emmit_signal;

public:
    knob_floatd(QList <float> default_values = {0, 1});
    ~knob_floatd();

	QList <float> get_values() const;
	float get_value(int dimension) const;
	void set_value(float value, int dimension = 0);
	void set_values(QList <float> values, bool emmit_signal = true);
signals:
	void changed(QList <float> values);
};

#endif // KNOB_FLOATD_H
