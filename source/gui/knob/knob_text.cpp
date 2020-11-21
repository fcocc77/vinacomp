#include <knob_text.hpp>

knob_text::knob_text(/* args */)
{

    QHBoxLayout *layout = new QHBoxLayout(this);

    QLabel *label = new QLabel("Text");
    layout->addWidget(label);

    QLineEdit *text = new QLineEdit();
    layout->addWidget(text);

    layout->addStretch();
}

knob_text::~knob_text()
{
}