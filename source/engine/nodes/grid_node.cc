#include <grid_node.h>

grid_node::grid_node()
{
    load_default_params("grid");
}

grid_node::~grid_node() {}

void grid_node::render(render_data *rdata, QJsonObject *params)
{
    float number = get(params, "number").toDouble();
    float size = get(params, "size").toDouble();
    bool round = get(params, "round").toBool();
    QString style = get(params, "line_style").toArray()[1].toString();

    QColor color = get_color(params);
    QColor background = Qt::black;

    int width = 1920;
    int height = 1080;

    // (*image) = QImage(width, height, QImage::Format_RGB32);
    // image->fill(background);

    // QPainter painter(image);
    // QPen pen(color, size);

    // if (style == "solid_line");
    // else if (style == "dash_line")
    // pen.setStyle(Qt::DashLine);
    // else if (style == "dot_line")
    // pen.setStyle(Qt::DotLine);
    // else if (style == "dash_dot_line")
    // pen.setStyle(Qt::DashDotLine);
    // else if (style == "dash_dot_dot_line")
    // pen.setStyle(Qt::DashDotDotLine);

    // if (round)
    // pen.setCapStyle(Qt::RoundCap);

    // if (style != "solid_line" && round)
    // painter.setRenderHint(QPainter::Antialiasing);

    // painter.setPen(pen);

    // float height_number = number;
    // float width_number = number;

    // float height_part = float( height ) / height_number;
    // float height_current = size / 2;
    // for (int h = 0; h <= height_number; h++)
    // {
    // painter.drawLine(0, height_current, width, height_current);
    // height_current += height_part;
    // }

    // float width_part = float( width ) / width_number;
    // float width_current = size / 2;
    // for (int w = 0; w <= width_number; w++)
    // {
    // painter.drawLine(width_current, 0, width_current, height);
    // width_current += width_part;
    // }
}
