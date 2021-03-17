#include <crop_node.h>

crop_node::crop_node()
{
    load_default_params( "crop" );
}

crop_node::~crop_node() {}

void crop_node::render( render_data *rdata, QJsonObject *params )
{
    QJsonArray box = get( params, "box" ).toArray();
    int x = box[ 0 ].toInt();
    int y = box[ 1 ].toInt();
    int w = box[ 2 ].toInt();
    int h = box[ 3 ].toInt();

    int width = rdata->image.cols;
    int height = rdata->image.rows;

    rdata->bbox.setRect( x, y, w, h );
    // cv::resize(rdata->image, rdata->image, cv::Size(500, 500), 0, 0, cv::INTER_CUBIC);

    if ( x < 0 )
    {
        w -= abs( x );
        x = 0;
    }
    if ( y < 0 )
    {
        h -= abs( y );
        y = 0;
    }

    // si el los datos del crop superan a la matriz de la imagen
    // limita los valores
    if ( w > width )
        w = width;
    if ( x + w > width )
        w -= x;

    if ( h > height )
        h = height;
    if ( y + h > height )
        h -= y;
    //
    //

    if ( w < 0 || h < 0 )
    {
        rdata->image = rdata->image( cv::Rect( 0, 0, 0, 0 ) );
        return;
    }

    int y_correct = height - h;
    int h_correct = height - y_correct;
    y_correct -= y;

    rdata->image = rdata->image( cv::Rect( x, y_correct, w, h_correct ) );
}
