#include <knob_floatd.h>

knob_floatd::knob_floatd(QList <float> default_values)
	: emmit_signal(true)
{
	this->setObjectName("knob_dimensions");
    QHBoxLayout *layout = new QHBoxLayout(this);
	this->set_knob_layout(layout);
	layout->setMargin(0);

    layout->addWidget(init_space);

	int dimensions = default_values.count();
	for ( int i = 0; i < dimensions; i++ )
	{
		float value = default_values.value(i);
		QLineEdit *dimension_edit = new QLineEdit(QString::number(value));

		connect(dimension_edit, &QLineEdit::editingFinished, this, [=](){
			QList <float> values;
			for ( int i = 0; i < dimensions; i++ )
				values.push_back(get_value(i));

			update_handler();
			if (emmit_signal)
				changed(values); // Signal
		});

		dimension_edit->setMaximumWidth(50);
		layout->addWidget(dimension_edit);
		dimensions_edits.push_back(dimension_edit);
	}

    layout->addStretch();
}

knob_floatd::~knob_floatd()
{
	for (QLineEdit *edit : dimensions_edits)
		delete edit;
}

float knob_floatd::get_value(int dimension) const
{
	if (dimension >= dimensions_edits.count())
		return 0;

	return dimensions_edits.value(dimension)->text().toDouble();
}

QList <float> knob_floatd::get_values() const
{
	QList <float> values;
	for (int i = 0; i < dimensions_edits.count(); i++)
		values.push_back( dimensions_edits.value(i)->text().toFloat() );

	return values;
}

void knob_floatd::set_value(float value, int dimension)
{
	if (dimension >= dimensions_edits.count())
		return;

	dimensions_edits.value(dimension)->setText(QString::number(value));
}

void knob_floatd::set_values(QList <float> values, bool _emmit_signal)
{
	emmit_signal = _emmit_signal;

	for (int i = 0; i < dimensions_edits.count(); i++)
		set_value(values.value(i), i);

	if (emmit_signal)
		changed(values); // Signal
}
