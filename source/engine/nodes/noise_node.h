#ifndef NOISE_NODE_H
#define NOISE_NODE_H

#include <QPainter>
#include <node_engine.h>

#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>
#include <vector>

class noise_node : public node_engine
{
private:
	std::vector<int> p;

	double noise(double x, double y, double z);
	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);
public:
	noise_node();
	~noise_node();

	void render(
		cv::Mat *image,
		QJsonObject *params,
		int frame,
		pair <int, int> &frame_range,
		QRect &bbox
	) override;
};

#endif // NOISE_NODE_H
