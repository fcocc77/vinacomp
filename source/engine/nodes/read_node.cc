#include <read_node.h>

read_node::read_node()
{
    load_default_params("read");
}

read_node::~read_node() {}

pair<int, int> read_node::get_frame_range(QJsonObject *params) const
{
    int first = get(params, "first").toArray()[0].toInt();
    int last = get(params, "last").toArray()[0].toInt();

    return {first, last};
}

void read_node::render(render_data *rdata, QJsonObject *params)
{
    QString file_path = get(params, "file").toString();
    QString _frame = frame_to_string(rdata->frame, 3);

    file_path.replace("###", _frame);

    // ! mejorar la velocidad de carga, ya que ahora tiene
    // dos iteraciones una para convertir a flotante y la otra para el flip

    rdata->channels["rgba"] = cv::imread(file_path.toStdString(), cv::IMREAD_COLOR);
    cv::Mat &image = rdata->channels["rgba"];

    image.convertTo(image, CV_32F);

    rdata->first_frame = get(params, "first").toArray()[0].toInt();
    rdata->last_frame = get(params, "last").toArray()[0].toInt();

    rdata->bbox.setRect(0, 0, image.cols, image.rows);
}

QString read_node::frame_to_string(int frame, int frame_digits)
{
    QString _frame = "0000000000" + QString::number(frame);
    return _frame.right(frame_digits);
}
