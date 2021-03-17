#include <saturation_node.h>

saturation_node::saturation_node()
{
    load_default_params( "saturation" );
}

saturation_node::~saturation_node() {}

void saturation_node::render( render_data *rdata, QJsonObject *params )
{
    float saturation = get( params, "saturation" ).toDouble();

    if ( saturation == 1 )
        return;

    for ( int y = 0; y < rdata->image.rows; y++ )
    {
        for ( int x = 0; x < rdata->image.cols; x++ )
        {
            cv::Vec3f &pixel = rdata->image.at<cv::Vec3f>( y, x );

            float &red = pixel[ 2 ];
            float &green = pixel[ 1 ];
            float &blue = pixel[ 0 ];

            // saturation
            float average = ( red + green + blue ) / 3;

            red = ( ( red - average ) * saturation ) + average;
            green = ( ( green - average ) * saturation ) + average;
            blue = ( ( blue - average ) * saturation ) + average;
        }
    }
}
