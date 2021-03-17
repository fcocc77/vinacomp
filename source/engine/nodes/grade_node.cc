#include <grade_node.h>

grade_node::grade_node()
{
    load_default_params( "grade" );
}

grade_node::~grade_node() {}

void grade_node::render( render_data *rdata, QJsonObject *params )
{
    float mix = get( params, "mix" ).toDouble();
    if ( mix == 0 )
        return;

    QJsonArray blackpoint = get( params, "blackpoint" ).toArray();
    QJsonArray whitepoint = get( params, "whitepoint" ).toArray();
    QJsonArray gain = get( params, "gain" ).toArray();
    QJsonArray multiply = get( params, "multiply" ).toArray();
    QJsonArray gamma = get( params, "gamma" ).toArray();
    QJsonArray offset = get( params, "offset" ).toArray();
    bool white_clamp = get( params, "white_clamp" ).toBool();
    bool black_clamp = get( params, "black_clamp" ).toBool();
    float saturation = get( params, "saturation" ).toDouble();

    float black_red = blackpoint[ 0 ].toDouble();
    float black_green = blackpoint[ 1 ].toDouble();
    float black_blue = blackpoint[ 2 ].toDouble();

    float white_red = whitepoint[ 0 ].toDouble();
    float white_green = whitepoint[ 1 ].toDouble();
    float white_blue = whitepoint[ 2 ].toDouble();

    float gain_red = gain[ 0 ].toDouble() * multiply[ 0 ].toDouble();
    float gain_green = gain[ 1 ].toDouble() * multiply[ 1 ].toDouble();
    float gain_blue = gain[ 2 ].toDouble() * multiply[ 2 ].toDouble();

    float offset_red = offset[ 0 ].toDouble() * 255;
    float offset_green = offset[ 1 ].toDouble() * 255;
    float offset_blue = offset[ 2 ].toDouble() * 255;

    float gamma_red = gamma[ 0 ].toDouble();
    float gamma_green = gamma[ 1 ].toDouble();
    float gamma_blue = gamma[ 2 ].toDouble();

    // levels
    float low = black_red * 255;
    float high = white_red * 255;
    float max = 255;
    float min = 0;
    float levels_alpha = ( ( max - min ) / ( high - low ) );
    //

    for ( int y = 0; y < rdata->image.rows; y++ )
    {
        for ( int x = 0; x < rdata->image.cols; x++ )
        {
            cv::Vec3f &pixel = rdata->image.at<cv::Vec3f>( y, x );

            float &_red = pixel[ 2 ];
            float &_green = pixel[ 1 ];
            float &_blue = pixel[ 0 ];

            float red = _red;
            float green = _green;
            float blue = _blue;

            // levels
            red = min + ( red - low ) * levels_alpha;
            green = min + ( green - low ) * levels_alpha;
            blue = min + ( blue - low ) * levels_alpha;
            //

            // gain
            red *= gain_red;
            green *= gain_green;
            blue *= gain_blue;
            //

            // offset
            red += offset_red;
            green += offset_green;
            blue += offset_blue;
            //

            // gamma
            if ( gamma_red != 1 )
                red = pow( ( red / 255.0 ), ( 1.0 / gamma_red ) ) * 255;
            if ( gamma_green != 1 )
                green = pow( ( green / 255.0 ), ( 1.0 / gamma_green ) ) * 255;
            if ( gamma_blue != 1 )
                blue = pow( ( blue / 255.0 ), ( 1.0 / gamma_blue ) ) * 255;
            //

            // saturation
            if ( saturation != 1 )
            {
                float average = ( red + green + blue ) / 3;

                red = ( ( red - average ) * saturation ) + average;
                green = ( ( green - average ) * saturation ) + average;
                blue = ( ( blue - average ) * saturation ) + average;
            }
            //

            // clamp
            if ( black_clamp )
            {
                if ( red < 0 )
                    red = 0;
                if ( green < 0 )
                    green = 0;
                if ( blue < 0 )
                    blue = 0;
            }
            if ( white_clamp )
            {
                if ( red > 255 )
                    red = 255;
                if ( green > 255 )
                    green = 255;
                if ( blue > 255 )
                    blue = 255;
            }
            //

            // mix
            if ( mix < 1 )
            {
                red = ( ( 1 - mix ) * _red ) + ( red * mix );
                green = ( ( 1 - mix ) * _green ) + ( green * mix );
                blue = ( ( 1 - mix ) * _blue ) + ( blue * mix );
            }
            //

            _red = red;
            _green = green;
            _blue = blue;
        }
    }
}
