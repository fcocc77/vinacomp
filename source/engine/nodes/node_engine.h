#ifndef NODE_ENGINE_H
#define NODE_ENGINE_H

#include <QJsonObject>
#include <opencv2/opencv.hpp>

#include <render_data.h>
#include <util.h>

class node_engine
{
private:
    QJsonObject default_params;

public:
    node_engine();
    ~node_engine();

    virtual void render( render_data *rdata, QJsonObject *params );

    QJsonValue get( QJsonObject *params, QString key ) const;

protected:
    void load_default_params( QString node_json_name );
    QColor get_color( QJsonObject *params, QString key = "color" ) const;
    QRect get_rect( QJsonObject *params, QString key ) const;
};

#endif // NODE_ENGINE_H
