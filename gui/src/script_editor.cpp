#include <script_editor.hpp>

script_editor::script_editor(/* args */)
{
    this->setObjectName("script_editor");
    setup_ui();
}

script_editor::~script_editor()
{
}

void script_editor::setup_ui()
{
    QVBoxLayout *layout = new QVBoxLayout();
    this->setLayout(layout);

    QWidget *tools = tools_setup_ui();

    QPlainTextEdit *output = new QPlainTextEdit();
    kgl::QCodeEditor *input = new kgl::QCodeEditor();

    // esto es para que el editor tenga colores
    kgl::QCodeEditorDesign design(":/design.xml");
    QList<kgl::QSyntaxRule> rules = kgl::QSyntaxRules::loadFromFile(":/rule_cpp.xml", design);
    input->setDesign(design);
    input->setRules(rules);
    input->setKeywords({"printf", "scanf", "atoi", "mbtoa", "strlen", "memcpy", "memset"});
    //

    layout->addWidget(tools);
    layout->addWidget(output);
    layout->addWidget(input);
}

QWidget *script_editor::tools_setup_ui()
{
    QWidget *widget = new QWidget();
    widget->setObjectName("tools");
    QHBoxLayout *layout = new QHBoxLayout();
    widget->setLayout(layout);

    int icon_size = 20;

    QPushButton *clear = new QPushButton();
    qt::set_icon(clear, "clear_script_a", icon_size);
    layout->addWidget(clear);

    QPushButton *run = new QPushButton();
    qt::set_icon(run, "run_script_a", icon_size);
    layout->addWidget(run);

    layout->addStretch();

    return widget;
}
