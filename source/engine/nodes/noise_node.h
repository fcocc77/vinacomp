#ifndef NOISE_NODE_H
#define NOISE_NODE_H

#include <QPainter>
#include <node_engine.h>

#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>
#include <vector>

class noise_node : public node_engine
{
private:
    std::vector<int> p;

    double noise( double x, double y, double z );
    double fade( double t );
    double lerp( double t, double a, double b );
    double grad( int hash, double x, double y, double z );

public:
    noise_node();
    ~noise_node();

    void render( render_data *rdata, QJsonObject *params ) override;
};

#endif // NOISE_NODE_H
