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
	void setup(QList <float> default_values);

public:
    knob_dimensions(QList <int> default_values = {0, 1});
    knob_dimensions(QList <float> default_values = {0, 1});
    ~knob_dimensions();

	float get_value(int dimension) const;
	void set_value(float value, int dimension = 0);
	void set_value(QList <float> values);
signals:
	void changed(QList <float> values);
};

#endif // KNOB_DIMENSIONS_H
