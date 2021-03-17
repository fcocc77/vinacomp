#include <tools.h>

tools::tools( int _icon_size, bool _vertical )
    : icon_size( _icon_size )
    , vertical( _vertical )
    , one_check_at_time( false )
{
    this->setObjectName( "tools" );

    if ( vertical )
    {
        this->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Expanding );
        layout = new QBoxLayout( QBoxLayout::TopToBottom, this );
        layout->setContentsMargins( 5, 15, 5, 15 );
    }
    else
    {
        this->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
        layout = new QBoxLayout( QBoxLayout::LeftToRight, this );
        layout->setContentsMargins( 15, 5, 15, 5 );
    }
    layout->setSpacing( 2 );
}

tools::~tools()
{
    for ( action *_action : actions )
        delete _action;
}

void tools::allow_one_check_at_time()
{
    // permite que solo se pueda hacer 1 solo 'check' a la vez
    one_check_at_time = true;
}

void tools::add_action( action *_action )
{
    actions.push_back( _action );

    QPushButton *button = _action->make_button( this, icon_size, one_check_at_time );
    layout->addWidget( button );
    _action->update();
}

void tools::set_checked_all( bool checked )
{
    for ( action *_action : actions )
        _action->set_checked( checked );
}

void tools::add_separator()
{
    int width = 20;
    QWidget *separator = new QWidget();
    if ( vertical )
    {
        separator->setMinimumHeight( width );
        separator->setObjectName( "tools_horizontal_separator" );
    }
    else
    {
        separator->setMinimumWidth( width );
        separator->setObjectName( "tools_vertical_separator" );
    }

    layout->addWidget( separator );
}

void tools::add_stretch()
{
    layout->addStretch();
}

void tools::add_widget( QWidget *widget )
{
    layout->addWidget( widget );
}

QBoxLayout *tools::get_layout() const
{
    return layout;
}
