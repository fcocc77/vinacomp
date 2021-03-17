#include <edge_detect_node.h>

edge_detect_node::edge_detect_node()
{
    load_default_params( "edge_detect" );
}

edge_detect_node::~edge_detect_node() {}

void edge_detect_node::render( render_data *rdata, QJsonObject *params ) {}
