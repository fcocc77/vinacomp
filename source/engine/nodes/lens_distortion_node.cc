#include <lens_distortion_node.h>

lens_distortion_node::lens_distortion_node()
{
	load_default_params("lens_distortion");
}

lens_distortion_node::~lens_distortion_node() { }

void lens_distortion_node::render( render_data *rdata, QJsonObject *params )
{
}
