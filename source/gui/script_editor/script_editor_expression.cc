#include <knob.h>
#include <script_editor.h>

void script_editor::set_knob(QWidget *__knob)
{
    current_knob = __knob;
    knob *_knob = static_cast<knob *>(__knob);

    exp_label->setText(_knob->get_name());
    editor->setPlainText(_knob->get_expression());

    show();
}

void script_editor::expression_ok()
{
    knob *_knob = static_cast<knob *>(current_knob);

    _knob->set_expression(editor->toPlainText());

    finish_expression_edit();
}

void script_editor::expression_cancel()
{
    finish_expression_edit();
}

void script_editor::finish_expression_edit()
{
    hide();
    current_knob = nullptr;
}
