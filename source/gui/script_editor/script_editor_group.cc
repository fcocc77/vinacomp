#include <group_gui.h>
#include <script_editor.h>
#include <util.h>

void script_editor::set_group_edit(QWidget *_group_gui)
{
    current_group = _group_gui;
    group_gui *group = static_cast<group_gui *>(current_group);

    group->set_open_script(true);

    editor->setPlainText(group->get_script());
    current_group_label->setText(group->get_name());

    save_action->set_icon("save", "normal");

    run_script_action->set_visible(false);
    save_action->set_visible(true);
    exit_action->set_visible(true);
    group_box->setVisible(true);

    group_separator_1->setVisible(true);
    group_separator_2->setVisible(true);

    cancel_exit();
}

void script_editor::save_script()
{
    if (!current_group)
        return;

    group_gui *group = static_cast<group_gui *>(current_group);
    group->save_script(editor->toPlainText());
}

void script_editor::exit_script()
{
    group_gui *group = static_cast<group_gui *>(current_group);

    if (editor->toPlainText() != group->get_script())
    {
        group_cancel_button->setVisible(true);
        save_exit_button->setVisible(true);
        cancel_exit_button->setVisible(true);
        return;
    }

    exit_group_edit();
}

void script_editor::save_and_exit()
{
    save_script();
    exit_script();
}

void script_editor::exit_group_edit()
{
    group_gui *group = static_cast<group_gui *>(current_group);
    if (group)
        group->set_open_script(false);

    current_group = nullptr;

    run_script_action->set_visible(true);
    save_action->set_visible(false);
    exit_action->set_visible(false);
    group_box->setVisible(false);
    group_separator_1->setVisible(false);
    group_separator_2->setVisible(false);

    editor->setPlainText(script);
    output->clear();
}

void script_editor::cancel_exit()
{
    group_cancel_button->setVisible(false);
    save_exit_button->setVisible(false);
    cancel_exit_button->setVisible(false);
}
