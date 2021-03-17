#include <text_node.h>

text_node::text_node()
{
    load_default_params( "text" );
}

text_node::~text_node() {}

void text_node::render( render_data *rdata, QJsonObject *params )
{
    QString text = get( params, "text" ).toString();
    int font_size = get( params, "font_size" ).toInt();
    QString custom_font = get( params, "custom_font" ).toString();

    QColor color = get_color( params );
    QColor background = Qt::black;

    int width = 1920;
    int height = 1080;

    // (*image) = QImage(width, height, QImage::Format_RGB32);
    // image->fill(background);

    // QPainter painter(image);
    // painter.setPen(QPen(color));
    // painter.setFont(QFont("Arial", font_size));
    // painter.scale(1.0, -1.0);

    // painter.drawText(0, 0, text);
}
