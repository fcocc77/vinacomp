#include <idistort_node.h>

idistort_node::idistort_node()
{
    load_default_params( "idistort" );
}

idistort_node::~idistort_node() {}

void idistort_node::render( render_data *rdata, QJsonObject *params ) {}
