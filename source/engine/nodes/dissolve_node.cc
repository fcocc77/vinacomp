#include <dissolve_node.h>

dissolve_node::dissolve_node()
{
    load_default_params( "dissolve" );
}

dissolve_node::~dissolve_node() {}

void dissolve_node::render( render_data *rdata, QJsonObject *params ) {}
