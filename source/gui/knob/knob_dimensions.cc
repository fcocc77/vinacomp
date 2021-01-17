#include <knob_dimensions.h>

knob_dimensions::knob_dimensions(QList <float> default_values, bool _integer)
	: integer(_integer)
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
			if (integer)
			{
				QList <int> values;
				for ( int i = 0; i < dimensions; i++ )
					values.push_back(get_value(i));
				changed_int(values);
			}
			else
			{
				QList <float> values;
				for ( int i = 0; i < dimensions; i++ )
					values.push_back(get_value(i));
				changed_float(values);
			}
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
