#include <blur_node.h>

blur_node::blur_node()
{
    load_default_params("blur");
}

blur_node::~blur_node() {}

void blur_node::render(render_data *rdata, QJsonObject *params)
{
    int blur_size = get(params, "size").toDouble();
    int filter_index = get(params, "filter").toArray()[0].toInt();

    cv::Mat &image = rdata->channels["rgba"];

    // cv::Mat img(image->height(), image->width(), CV_8UC4, image->bits(), image->bytesPerLine());
    if (blur_size > 0)
    {
        if (filter_index == 0)
            cv::blur(image, image, cv::Size(blur_size, blur_size));
        else if (filter_index == 3)
            cv::GaussianBlur(image, image, cv::Size(99, 99), 0);
    }
}
