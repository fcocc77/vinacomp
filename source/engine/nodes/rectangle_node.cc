#include <rectangle_node.h>

rectangle_node::rectangle_node()
{
    load_default_params( "rectangle" );
}

rectangle_node::~rectangle_node() {}

void rectangle_node::render( render_data *rdata, QJsonObject *params ) {}
