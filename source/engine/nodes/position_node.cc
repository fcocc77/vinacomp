#include <position_node.h>

position_node::position_node()
{
    load_default_params("position");
}

position_node::~position_node() {}

void position_node::render(render_data *rdata, QJsonObject *params)
{
    int x = get(params, "translate").toArray()[0].toInt();
    int y = get(params, "translate").toArray()[1].toInt();

    QRect &bbox = rdata->bbox;
    bbox.setRect(bbox.x() + x, bbox.y() + y, bbox.width(), bbox.height());
}
