#include <knob_dimensions.h>

knob_dimensions::knob_dimensions(QList <float> default_values)
{
	setup(default_values);
}

knob_dimensions::knob_dimensions(QList <int> default_values)
{
	// convierte la lista 'int' a 'float' porque al final se retornara en 'int'
	// asi que da igual que este en 'float'
	QList <float> float_list;
	for (int value : default_values)
		float_list.push_back(value);

	setup(float_list);
}

void knob_dimensions::setup(QList <float> default_values)
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
			changed(values); // Signal
		});
		dimension_edit->setMaximumWidth(50);
		layout->addWidget(dimension_edit);

		dimensions_edits.push_back(dimension_edit);
	}

    layout->addStretch();
}

knob_dimensions::~knob_dimensions()
{
	for (QLineEdit *edit : dimensions_edits)
		delete edit;
}

float knob_dimensions::get_value(int dimension) const
{
	if (dimension >= dimensions_edits.count())
		return 0;

	return dimensions_edits.value(dimension)->text().toDouble();
}

void knob_dimensions::set_value(float value, int dimension)
{
	if (dimension >= dimensions_edits.count())
		return;

	dimensions_edits.value(dimension)->setText(QString::number(value));
}

void knob_dimensions::set_value(QList <float> values)
{
	for (int i = 0; i < dimensions_edits.count(); i++)
		set_value(values.value(i), i);

	changed(values); // Signal
}
