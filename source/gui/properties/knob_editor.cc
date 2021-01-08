#include <knob_editor.h>

knob_editor::knob_editor()
{
	layout = new QVBoxLayout(this);

	knobs_list = new QTreeWidget(this);
	layout->addWidget(knobs_list);
}

knob_editor::~knob_editor()
{

}
