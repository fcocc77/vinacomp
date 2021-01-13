#include <knob_dimensions.h>

knob_dimensions::knob_dimensions(int _dimensions, QList <float> default_values)
	: dimensions(_dimensions)
{
	this->setObjectName("knob_dimensions");
    QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setMargin(0);

    layout->addWidget(init_space);

	for ( int i = 0; i < dimensions; i++ )
	{
		float value = default_values.value(i);
		QLineEdit *dimension_edit = new QLineEdit(QString::number(value));
		connect(dimension_edit, &QLineEdit::editingFinished, this, [=](){
			changed(
				get_value(0),
				get_value(1),
				get_value(2),
				get_value(3)
			);
		});
		dimension_edit->setMaximumWidth(50);
		layout->addWidget(dimension_edit);

		dimensions_edits.push_back(dimension_edit);
	}

    layout->addStretch();

    animation = new QPushButton();
    qt::set_icon(animation, "key_a", icon_size);
    layout->addWidget(animation);
}

knob_dimensions::~knob_dimensions()
{
	for (QLineEdit *edit : dimensions_edits)
		delete edit;

	delete animation;
}

float knob_dimensions::get_value(int dimension) const
{
	if (dimension >= dimensions_edits.count())
		return 0;

	return dimensions_edits.value(dimension)->text().toDouble();
}
