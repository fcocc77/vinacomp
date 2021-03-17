#include <clamp_node.h>

clamp_node::clamp_node()
{
    load_default_params( "clamp" );
}

clamp_node::~clamp_node() {}

void clamp_node::render( render_data *rdata, QJsonObject *params ) {}
