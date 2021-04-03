#include <constant_node.h>

constant_node::constant_node()
{
    load_default_params("constant");
}

constant_node::~constant_node() {}

void constant_node::render(render_data *rdata, QJsonObject *params)
{
    auto layer = get_layer(params);

    if (rdata->layer == layer.name)
    {
        rdata->first_frame = get(params, "frame_range").toArray()[0].toInt();
        rdata->last_frame = get(params, "frame_range").toArray()[1].toInt();

        int x = get(params, "format").toArray()[1].toArray()[0].toInt();
        int y = get(params, "format").toArray()[1].toArray()[1].toInt();

        QColor color = get_color(params);

        int red, green, blue, alpha;

        red = 0;
        green = 0;
        blue = 0;
        alpha = 0;

        if (layer.red)
            red = color.red();
        if (layer.green)
            green = color.green();
        if (layer.blue)
            blue = color.blue();
        if (layer.alpha)
            alpha = color.alpha();

        cv::Mat3f solid(1080, 1920);
        solid.setTo(cv::Scalar(blue, green, red));

        rdata->image = solid;
    }
}
