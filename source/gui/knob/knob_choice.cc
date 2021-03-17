#include <knob_choice.h>

knob_choice::knob_choice( QList<pair<QString, QVariant>> items, int default_index )
{
    this->setObjectName( "knob_choice" );
    QHBoxLayout *layout = new QHBoxLayout( this );
    layout->setMargin( 0 );

    layout->addWidget( init_space );

    choice = new combo_box( items, default_index );
    connect( choice, &combo_box::changed, this, &knob_choice::changed );

    layout->addWidget( choice );
    layout->addStretch();
}

knob_choice::~knob_choice() {}

int knob_choice::get_index() const
{
    return choice->get_index();
}

QVariant knob_choice::get_value() const
{
    return choice->get_value();
}
