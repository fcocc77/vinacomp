#include <knob_check_box.h>

knob_check_box::knob_check_box( QString _label, bool default_value )

    : label( _label )
    , checked( default_value )
{
    this->setObjectName( "knob_check_box" );

    QHBoxLayout *layout = new QHBoxLayout( this );
    layout->setMargin( 0 );

    layout->addWidget( init_space );

    checkbox = new QCheckBox( this );
    connect( checkbox, &QCheckBox::stateChanged, this, [=]( int state ) {
        checked = state != 0;
        changed( checked ); // Signal
    } );
    set_check( checked );
    layout->addWidget( checkbox );

    label_widget = new QLabel( this );
    label_widget->setText( label );
    layout->addWidget( label_widget );

    layout->addStretch();
}

knob_check_box::~knob_check_box() {}

void knob_check_box::set_check( bool value )
{
    if ( value == 0 )
        checkbox->setCheckState( Qt::CheckState( 0 ) );
    else
        checkbox->setCheckState( Qt::CheckState( 2 ) );
}

bool knob_check_box::is_checked() const
{
    return checked;
}

void knob_check_box::mousePressEvent( QMouseEvent *event )
{
    set_check( !checked );
}
