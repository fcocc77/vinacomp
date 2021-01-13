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
	int dimensions;
	QList < QLineEdit * > dimensions_edits;
	QPushButton *animation;

public:
    knob_dimensions(int _dimensions = 2, QList <float> default_values = {0, 1});
    ~knob_dimensions();

	float get_value(int dimension) const;
signals:
	void changed(float d1, float d2, float d3, float d4);
};

#endif // KNOB_DIMENSIONS_H
