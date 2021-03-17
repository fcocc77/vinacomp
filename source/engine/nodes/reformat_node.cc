#include <reformat_node.h>

reformat_node::reformat_node()
{
    load_default_params("reformat");
}

reformat_node::~reformat_node() {}

void reformat_node::render(render_data *rdata, QJsonObject *params)
{
    int index = get(params, "type").toArray()[0].toInt();
    int filter_index = get(params, "filter").toArray()[0].toInt();

    // // Filtro
    // Qt::TransformationMode filter;
    // if (filter_index == 0)
    // filter = Qt::FastTransformation;
    // else if (filter_index == 1)
    // filter = Qt::SmoothTransformation;
    // //

    // int width = image->width();
    // int height = image->height();
    // int x, y;

    // if (index == 0) // Format
    // {
    // x = get(params, "format").toArray()[1].toArray()[0].toInt();
    // y = get(params, "format").toArray()[1].toArray()[1].toInt();
    // }
    // else if (index == 1) // Scale
    // {
    // float scale = get(params, "scale").toDouble();

    // x = width * scale;
    // y = height * scale;
    // }
    // else if (index == 2) // Custom
    // {
    // x = get(params, "custom").toArray()[0].toInt();
    // y = get(params, "custom").toArray()[1].toInt();
    // }

    // if (x != width || y != height)
    // (*image) = image->scaled(x, y, Qt::IgnoreAspectRatio, filter);
}
