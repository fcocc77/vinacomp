#include <ramp_node.h>

ramp_node::ramp_node()
{
    load_default_params( "ramp" );
}

ramp_node::~ramp_node() {}

void ramp_node::render( render_data *rdata, QJsonObject *params )
{
    QJsonArray point0 = get( params, "point0" ).toArray();
    QJsonArray point1 = get( params, "point1" ).toArray();
    QPoint p0 = {point0[ 0 ].toInt(), point0[ 1 ].toInt()};
    QPoint p1 = {point1[ 0 ].toInt(), point1[ 1 ].toInt()};

    int x = get( params, "format" ).toArray()[ 1 ].toArray()[ 0 ].toInt();
    int y = get( params, "format" ).toArray()[ 1 ].toArray()[ 1 ].toInt();

    QColor color = get_color( params );
    float red = color.red();
    float green = color.green();
    float blue = color.blue();

    cv::Mat3f ramp( 1080, 1920 );

    for ( int y = 0; y < ramp.rows; y++ )
    {
        float _red = ( red * y ) / ramp.rows;
        float _green = ( green * y ) / ramp.rows;
        float _blue = ( blue * y ) / ramp.rows;

        ramp.row( y ).setTo( cv::Vec3f( _blue, _green, _red ) );
    }

    rdata->image = ramp;
}
