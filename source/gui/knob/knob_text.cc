#include <knob_text.h>

knob_text::knob_text( QString default_text )
{
    QHBoxLayout *layout = new QHBoxLayout( this );
    layout->setMargin( 0 );
    layout->addWidget( init_space );

    QLineEdit *text = new QLineEdit( default_text );
    connect( text, &QLineEdit::editingFinished, this, [=]() { changed( text->text() ); } );

    layout->addWidget( text );
}

knob_text::~knob_text() {}
