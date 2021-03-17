#include <shuffle_gui.h>

void shuffle_gui::draw_bezier( QPainter &painter, QPoint src, QPoint dst )
{
    float distance_x = dst.x() - src.x();
    float mid_distance_x = distance_x / 1.7;

    QPointF middle_src = {src.x() + mid_distance_x, src.y()};
    QPointF middle_dst = {dst.x() - mid_distance_x, dst.y()};

    int divisions = 30;

    QPointF last_point = src;
    for ( int i = 1; i <= divisions; i++ )
    {
        float value = float( i ) / divisions;
        QPointF point =
            cubic_bezier( QPointF( src ), middle_src, middle_dst, QPointF( dst ), value );

        painter.drawLine( last_point, point );
        last_point = point;
    }
}

void shuffle_gui::paintEvent( QPaintEvent *event )
{
    QPainter painter( this );
    painter.setRenderHints( QPainter::Antialiasing );

    int radius = 5;
    int edge = 2;

    auto draw_in_connector = [=]( in_layer *layer, QPainter &painter ) {
        int x = layer->x() + layer->width() + 20;
        int y = layer->y() + 30;

        for ( in_connector *conn : layer->get_connectors() )
        {
            painter.setPen( QPen( conn->get_color(), edge ) );

            y += 20;
            QPoint position = {x, y};
            conn->set_position( position );

            if ( dragging_input == conn )
            {
                painter.setBrush( QBrush( conn->get_color() ) );
                draw_bezier( painter, position, mouse_position );
            }
            else
            {
                painter.setBrush( Qt::transparent );
            }

            if ( conn->is_connected() )
            {
                painter.setBrush( QBrush( conn->get_color() ) );
                for ( out_connector *out_conn : conn->get_outputs() )
                    draw_bezier( painter, position, out_conn->get_position() );
            }

            painter.drawEllipse( position, radius, radius );
        }
    };

    auto draw_out_connector = [=]( out_layer *layer, QPainter &painter ) {
        int x = this->width() - 220;
        int y = layer->y() + 30;

        for ( out_connector *conn : layer->get_connectors() )
        {
            painter.setPen( QPen( conn->get_color(), edge ) );

            if ( conn->is_connected() )
                painter.setBrush( QBrush( conn->get_color() ) );
            else
                painter.setBrush( Qt::transparent );

            y += 20;
            QPoint position = {x, y};
            conn->set_position( position );

            painter.drawEllipse( position, radius, radius );
        }
    };

    draw_in_connector( in_layer_a, painter );
    draw_in_connector( in_layer_b, painter );

    draw_out_connector( out_layer_a, painter );
    draw_out_connector( out_layer_b, painter );

    QWidget::paintEvent( event );
}
