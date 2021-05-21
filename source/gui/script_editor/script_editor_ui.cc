#include <knob_integer.h>
#include <qt.h>
#include <script_editor.h>
#include <slider.h>
#include <tools.h>

void script_editor::setup_ui()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);

    if (expression_editor)
    {
        expression_editor_bar = set_expression_editor_bar();
        layout->addWidget(expression_editor_bar);
    }
    else
    {
        QWidget *_tools = tools_setup_ui();
        layout->addWidget(_tools);
    }

    output = new QTextEdit();
    output->setReadOnly(true);
    output->setObjectName("output");
    QCodeEditor *input = code_editor();


    QSplitter *splitter = new QSplitter();
    splitter->setOrientation(Qt::Vertical);

    splitter->addWidget(output);
    splitter->addWidget(input);

    splitter->setSizes({100, 200});

    layout->addWidget(splitter);
}

QWidget *script_editor::tools_setup_ui()
{
    tools *_tools = new tools();

    action *clear_action = new action("Clear", "", "clear_script");
    action *run_script_action = new action("Run Script", "", "run_script");

    clear_action->connect_to(this, [=]() { output->clear(); });
    run_script_action->connect_to(this, [=]() { run_script(); });

    knob_integer *font_size_slider = new knob_integer({}, 7, 50, 14);
    connect(font_size_slider, &knob_integer::changed, this, [=](int value) {
        QString style = "font-size: " + QString::number(value) + "px;";
        output->setStyleSheet(style);
        editor->setStyleSheet(style);
    });
    font_size_slider->set_animatable(false);
    font_size_slider->set_init_space(100);
    font_size_slider->set_init_label_text("Font Size");

    _tools->add_action(clear_action);
    _tools->add_action(run_script_action);

    _tools->add_stretch();

    _tools->add_widget(font_size_slider);

    return _tools;
}

QCodeEditor *script_editor::code_editor()
{
    editor = new QCodeEditor();
    editor->setObjectName("code_editor");

    QPythonHighlighter *python_highlighter = new QPythonHighlighter();
    editor->setHighlighter(python_highlighter);

    connect(editor, &QTextEdit::textChanged, this, [this]() {
        if (expression_editor)
            run_expression();
        else if (project)
            // guarda el script escrito en el proyecto
            project->insert("script_editor", editor->toPlainText());
    });

    return editor;
}

QWidget *script_editor::set_expression_editor_bar()
{
    QWidget *widget = new QWidget;
    widget->setObjectName("expression_editor_bar");

    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(10, 4, 10, 4);

    QLabel *exp_label_init = new QLabel("Editing Expression: ");
    exp_label = new QLabel("'param name'");
    exp_label->setObjectName("exp_label");
    button *code_icon = new button();
    code_icon->setObjectName("code_icon");
    code_icon->set_icon("code");

    QPushButton *ok_button = new QPushButton("OK");
    QPushButton *cancel_button = new QPushButton("Cancel");

    connect(ok_button, &QPushButton::clicked, this,
            &script_editor::expression_ok);
    connect(cancel_button, &QPushButton::clicked, this,
            &script_editor::expression_cancel);

    layout->addWidget(code_icon);
    layout->addWidget(exp_label_init);
    layout->addWidget(exp_label);
    layout->addStretch();
    layout->addWidget(cancel_button);
    layout->addWidget(ok_button);

    return widget;
}
