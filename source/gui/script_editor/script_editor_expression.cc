#include <knob.h>
#include <knob_slider.h>
#include <script_editor.h>
#include <util.h>

void script_editor::set_knob(QWidget *__knob)
{
    current_knob = __knob;
    knob *_knob = static_cast<knob *>(__knob);

    exp_label->setText(_knob->get_name());

    QString expression = _knob->get_expression();
    if (expression.isEmpty())
        editor->setPlainText("0");
    else
        editor->setPlainText(expression);

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

void script_editor::run_expression()
{
    QString code = editor->toPlainText();
    bool one_line = !code.contains("\n");

    QString out;

    if (one_line)
    {
        if (code.contains("ret"))
            out = python_run(code + "\nprint(ret)");
        else
            out = python_run("print(" + code + ")");
    }
    else
        out = python_run("ret=''\n" + code + "\nprint(ret)");

    knob_slider *_knob_slider = dynamic_cast<knob_slider *>(current_knob);
    knob *_knob = dynamic_cast<knob *>(current_knob);

    bool error = out.contains("Error:");

    if (out.count() <= 1)
    {
        out = "You have to have the variable 'ret' for the return, and if it "
              "is a one line expression you just need some value";
        error = true;
    }

    if (error)
    {
        output->setTextColor({200, 0, 0});
        output->setText(out);

        if (_knob)
            _knob->set_error(true);
    }
    else
    {
        output->setTextColor({100, 150, 200});
        output->setText(out);

        if (_knob)
            _knob->set_error(false);

        float value = out.toFloat();
        if (_knob_slider)
            _knob_slider->set_value(value);
    }
}
