#include <nodes_bar.h>

nodes_bar::nodes_bar( QWidget *_parent, maker *__maker, nodes_load *_nodes )
    : parent( _parent )
    , _maker( __maker )
    , nodes( _nodes )

{
    this->setObjectName( "nodes_bar" );
    layout = new QHBoxLayout( this );
    layout->setMargin( 4 );

    setup_ui();
}

nodes_bar::~nodes_bar() {}

void nodes_bar::setup_ui()
{
    int icon_size = 20;

    add_menu( "image", "image" );
    add_menu( "draw", "brush" );
    add_menu( "time", "time" );
    add_menu( "channel", "channel" );
    add_menu( "color", "color" );
    add_menu( "filter", "filter" );
    add_menu( "keyer", "keyer" );
    add_menu( "merge", "merge" );
    add_menu( "transform", "transform" );
    add_menu( "other", "other" );

    layout->addStretch();

    QLineEdit *find_node = new QLineEdit();
    QLabel *find_node_label = new QLabel( "Search Node" );

    layout->addWidget( find_node_label );
    layout->addWidget( find_node );

    layout->addStretch();

    QPushButton *show_exp_link = new QPushButton();
    qt::set_icon( show_exp_link, "link_off_a", icon_size );
    layout->addWidget( show_exp_link );

    QPushButton *show_grid = new QPushButton();
    qt::set_icon( show_grid, "grid_a", icon_size );
    layout->addWidget( show_grid );
}

void nodes_bar::add_menu( QString group, QString icon_group )
{
    button *popup_button = new button( this );
    popup_button->set_icon( icon_group );
    popup_button->setObjectName( "nodes_bar_button" );

    menu *_menu = new menu( popup_button );

    connect( popup_button, &QPushButton::pressed, this, [=] {
        popup_button->set_hover_icon();
        _menu->exec( popup_button->mapToGlobal( {0, this->height() - 4} ) );
    } );

    for ( QJsonValue value : nodes->get_effects( group ) )
    {
        QJsonObject effect = value.toObject();
        QString label = effect[ "label" ].toString();
        QString id = effect[ "id" ].toString();
        QString icon = effect[ "icon" ].toString();

        QString shortcut;
        if ( id == "grade" )
            shortcut = "G";
        else if ( id == "transform" )
            shortcut = "T";
        else if ( id == "blur" )
            shortcut = "B";
        else if ( id == "merge" )
            shortcut = "M";
        else if ( id == "read" )
            shortcut = "R";
        else if ( id == "write" )
            shortcut = "W";
        else if ( id == "copy" )
            shortcut = "K";

        action *effect_action = new action( label, shortcut, icon );
        _menu->addAction( effect_action );

        effect_action->connect_to( parent, [=]() { _maker->create_fx( id ); } );
    }

    layout->addWidget( popup_button );
}

void menu::hideEvent( QHideEvent *event )
{
    popup_button->set_normal_icon();
    QMenu::hideEvent( event );
}

void menu::mouseMoveEvent( QMouseEvent *event )
{
    // si el boton de este menu no es igual al que esta bajo el cursor,
    // cierra este menu y abre el que corresponde.
    QPoint global_position = this->mapToGlobal( event->pos() );

    QWidget *widget = qApp->widgetAt( global_position );
    button *_button = static_cast<button *>( widget );
    if ( _button )
        if ( _button->objectName() == "nodes_bar_button" )
            if ( _button != popup_button )
            {
                this->hide();

                _button->set_hover_icon();
                _button->click();
            }

    QMenu::mouseMoveEvent( event );
}
