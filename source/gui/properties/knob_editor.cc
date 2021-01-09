#include <knob_editor.h>

knob_editor::knob_editor()
{
	layout = new QVBoxLayout(this);

	// knobs_list = new QTreeWidget(this);
	// layout->addWidget(knobs_list);

	tools *tools_bar = new tools(20);
	layout->addWidget(tools_bar);
	layout->setMargin(0);

	action *float_knob_action = new action("Floating Knob", "", "float");
	float_knob_action->set_checkable(true);
	float_knob_action->connect_to(this, [](){ });
	tools_bar->add_action(float_knob_action);

	action *int_knob_action = new action("Integer Knob", "", "int");
	int_knob_action->set_checkable(true);
	int_knob_action->connect_to(this, [](){ });
	tools_bar->add_action(int_knob_action);

	action *color_knob_action = new action("Color Knob", "", "color");
	color_knob_action->set_checkable(true);
	color_knob_action->connect_to(this, [](){ });
	tools_bar->add_action(color_knob_action);

	action *button_knob_action = new action("Button Knob", "", "button");
	button_knob_action->set_checkable(true);
	button_knob_action->connect_to(this, [](){ });
	tools_bar->add_action(button_knob_action);

	action *combo_box_knob_action = new action("Choice Knob", "", "combo_box");
	combo_box_knob_action->set_checkable(true);
	combo_box_knob_action->connect_to(this, [](){ });
	tools_bar->add_action(combo_box_knob_action);

	action *check_box_knob_action = new action("CheckBox Knob", "", "check_box");
	check_box_knob_action->set_checkable(true);
	check_box_knob_action->connect_to(this, [](){ });
	tools_bar->add_action(check_box_knob_action);

	action *text_knob_action = new action("Text Knob", "", "text");
	text_knob_action->set_checkable(true);
	text_knob_action->connect_to(this, [](){ });
	tools_bar->add_action(text_knob_action);

	action *file_knob_action = new action("File Knob", "", "create_new_folder");
	file_knob_action->set_checkable(true);
	file_knob_action->connect_to(this, [](){ });
	tools_bar->add_action(file_knob_action);

	action *position_knob_action = new action("Position Knob", "", "position");
	position_knob_action->set_checkable(true);
	position_knob_action->connect_to(this, [](){ });
	tools_bar->add_action(position_knob_action);

	action *label_knob_action = new action("Label Knob", "", "label");
	label_knob_action->set_checkable(true);
	label_knob_action->connect_to(this, [](){ });
	tools_bar->add_action(label_knob_action);

	action *group_knob_action = new action("Group Knob", "", "group");
	group_knob_action->set_checkable(true);
	group_knob_action->connect_to(this, [](){ });
	tools_bar->add_action(group_knob_action);

	action *separator_knob_action = new action("Separator Knob", "", "separator");
	separator_knob_action->set_checkable(true);
	separator_knob_action->connect_to(this, [](){ });
	tools_bar->add_action(separator_knob_action);

	tools_bar->add_stretch();

	// Caja de edicion
	QWidget *edit_box = new QWidget(this);
	QHBoxLayout *edit_box_layout = new QHBoxLayout(edit_box);
	layout->addWidget(edit_box);


	QLineEdit *knob_name = new QLineEdit(this);
	QLineEdit *knob_label = new QLineEdit(this);
	QLineEdit *minimum_edit = new QLineEdit(this);
	QLineEdit *maximum_edit = new QLineEdit(this);
	QLabel *new_line_label = new QLabel("New Line");
	QCheckBox *new_line_check = new QCheckBox(this);

	minimum_edit->setMaximumWidth(70);
	maximum_edit->setMaximumWidth(70);

	knob_name->setPlaceholderText("Name Knob");
	knob_label->setPlaceholderText("Label Knob");
	minimum_edit->setPlaceholderText("Min: 0");
	maximum_edit->setPlaceholderText("Max: 100");

	QPushButton *add_button = new QPushButton(this);
	qt::set_icon(add_button, "add", 20);

	edit_box_layout->addWidget(knob_name);
	edit_box_layout->addWidget(knob_label);
	edit_box_layout->addWidget(new_line_check);
	edit_box_layout->addWidget(new_line_label);
	edit_box_layout->addWidget(minimum_edit);
	edit_box_layout->addWidget(maximum_edit);
	edit_box_layout->addWidget(add_button);
}

knob_editor::~knob_editor()
{

}
