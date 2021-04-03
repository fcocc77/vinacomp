#include <sharpen_node.h>

sharpen_node::sharpen_node()
{
    load_default_params("sharpen");
}

sharpen_node::~sharpen_node() {}

void sharpen_node::render(render_data *rdata, QJsonObject *params)
{
    float amount = get(params, "amount").toDouble();
    float size = get(params, "size").toDouble();

    // cv::Mat img(image->height(), image->width(), CV_8UC4, image->bits(), image->bytesPerLine());
    // cv::blur(img, img, cv::Size(size, size));

    // cv::GaussianBlur(frame, image, cv::Size(0, 0), 3);
    // cv::addWeighted(0, 1.5, img, -0.5, 0, img);

    // cv::Mat sharpenedLena;
    cv::Mat &image = rdata->image;

    cv::Mat kernel = (cv::Mat_<float>(3, 3) << 0, -1, 0, -1, 4, -1, 0, -1, 0);
    cv::filter2D(image, image, image.depth(), kernel);
}
