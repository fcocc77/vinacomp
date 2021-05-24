#include <button.h>
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

    // Creacion
    action *clear_action = new action("Clear Output", "", "clear_script");
    run_script_action = new action("Run Script", "", "run_script");
    save_action = new action("Save Script", "", "save");
    exit_action = new action("Exit Script", "", "exit");
    knob_integer *font_size_slider = new knob_integer({}, 7, 50, 14);
    group_box = new QWidget;
    QHBoxLayout *group_box_layout = new QHBoxLayout(group_box);
    QLabel *group_label = new QLabel("Editing Group Script: ");
    current_group_label = new QLabel();
    save_exit_button = new button();
    cancel_exit_button = new button();

    // Opciones
    font_size_slider->set_animatable(false);
    font_size_slider->set_init_space(100);
    font_size_slider->set_init_label_text("Font Size");
    font_size_slider->setMaximumWidth(500);

    save_action->set_visible(false);
    exit_action->set_visible(false);

    group_box_layout->setMargin(0);
    group_box->hide();
    group_box->setObjectName("group_box");
    save_exit_button->hide();
    cancel_exit_button->hide();
    current_group_label->setObjectName("current_group_label");
    save_exit_button->setText("Save and Exit");
    cancel_exit_button->setText("Exit");

    // Conecciones
    connect(font_size_slider, &knob_integer::changed, this, [=](int value) {
        QString style = "font-size: " + QString::number(value) + "px;";
        output->setStyleSheet(style);
        editor->setStyleSheet(style);
    });

    clear_action->connect_to(this, [=]() { output->clear(); });
    run_script_action->connect_to(this, [=]() { run_script(); });

    save_action->connect_to(this, [=]() { save_script(); });
    exit_action->connect_to(this, [=]() { exit_script(); });

    connect(save_exit_button, &button::clicked, this,
            &script_editor::save_and_exit);

    connect(cancel_exit_button, &button::clicked, this,
            &script_editor::cancel_and_exit);

    // Layout
    group_box_layout->addWidget(save_exit_button);
    group_box_layout->addWidget(cancel_exit_button);
    group_box_layout->addWidget(group_label);
    group_box_layout->addWidget(current_group_label);

    _tools->add_action(clear_action);
    _tools->add_action(run_script_action);

    group_separator_1 = _tools->add_separator();

    _tools->add_action(save_action);
    _tools->add_action(exit_action);

    group_separator_2 = _tools->add_separator();

    _tools->add_widget(group_box);

    _tools->add_stretch();

    _tools->add_widget(font_size_slider);

    group_separator_1->hide();
    group_separator_2->hide();

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
        else if (current_group)
            save_action->set_icon("save", "checked");
        else if (project)
        {
            // guarda el script escrito en el proyecto
            script = editor->toPlainText();
            project->insert("script_editor", script);
        }
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
