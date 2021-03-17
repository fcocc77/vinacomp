#include <knob_integer.h>

knob_integer::knob_integer( int min, int max, int default_value )
{
    this->setObjectName( "knob_integer" );
    QHBoxLayout *layout = new QHBoxLayout( this );
    layout->setMargin( 0 );

    layout->addWidget( init_space );

    value_edit = new QLineEdit( QString::number( default_value ) );
    connect( value_edit, &QLineEdit::editingFinished, this, [=] {
        int value = value_edit->text().toInt();
        set_value( value );
    } );
    value_edit->setMaximumWidth( 50 );
    layout->addWidget( value_edit );

    _slider = new slider( min, max, default_value, false );
    connect( _slider, &slider::moved, this, [=]( int value ) {
        value_edit->setText( QString::number( value ) );
        changed( value );
    } );
    layout->addWidget( _slider );

    QPushButton *animation = new QPushButton();
    qt::set_icon( animation, "key_a", icon_size );
    layout->addWidget( animation );
}

knob_integer::~knob_integer() {}

void knob_integer::set_value( int value )
{
    value_edit->setText( QString::number( value ) );
    _slider->set_value( value );
}
