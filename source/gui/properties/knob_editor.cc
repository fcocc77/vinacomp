#include <knob_editor.h>

knob_editor::knob_editor()
{
	layout = new QVBoxLayout(this);

	// knobs_list = new QTreeWidget(this);
	// layout->addWidget(knobs_list);

	tools *tools_bar = new tools();
	layout->addWidget(tools_bar);

	action *float_knob_action = new action("Floating Knob", "", "float_a");
	tools_bar->add_action(float_knob_action);

	action *int_knob_action = new action("Integer Knob", "", "int_a");
	tools_bar->add_action(int_knob_action);

	action *color_knob_action = new action("Color Knob", "", "color_a");
	tools_bar->add_action(color_knob_action);

	action *button_knob_action = new action("Button Knob", "", "button_a");
	tools_bar->add_action(button_knob_action);

	action *combo_box_knob_action = new action("Choice Knob", "", "combo_box_a");
	tools_bar->add_action(combo_box_knob_action);

	action *check_box_knob_action = new action("CheckBox Knob", "", "check_box_a");
	tools_bar->add_action(check_box_knob_action);

	action *text_knob_action = new action("Text Knob", "", "text_a");
	tools_bar->add_action(text_knob_action);

	action *file_knob_action = new action("File Knob", "", "create_new_folder_a");
	tools_bar->add_action(file_knob_action);

	action *position_knob_action = new action("Position Knob", "", "position_a");
	tools_bar->add_action(position_knob_action);

	action *label_knob_action = new action("Label Knob", "", "label_a");
	tools_bar->add_action(label_knob_action);

	action *group_knob_action = new action("Group Knob", "", "group_a");
	tools_bar->add_action(group_knob_action);

	action *separator_knob_action = new action("Separator Knob", "", "separator_a");
	tools_bar->add_action(separator_knob_action);

	tools_bar->add_stretch();
}

knob_editor::~knob_editor()
{

}
