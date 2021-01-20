#ifndef KNOB_DIMENSIONS_H
#define KNOB_DIMENSIONS_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>

#include <slider.h>
#include <qt.h>
#include <knob.h>

class knob_dimensions : public knob
{
	Q_OBJECT
private:
	QList < QLineEdit * > dimensions_edits;
	void setup(QList <float> default_values, bool integer);

public:
    knob_dimensions(QList <int> default_values = {0, 1});
    knob_dimensions(QList <float> default_values = {0, 1});
    ~knob_dimensions();

	float get_value(int dimension) const;
signals:
	void changed_float(QList <float> values);
	void changed_int(QList <int> values);
};

#endif // KNOB_DIMENSIONS_H
