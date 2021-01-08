#ifndef SLIDER_HPP
#define SLIDER_HPP

#include <QSlider>

#include <util.h>

class slider : public QSlider
{
	Q_OBJECT
private:
	bool floating;
	float min;
	float max;
	float float_interval;

public:
    slider(float _min = 0,
			float _max = 100,
			bool _floating = true);
    ~slider();

	void set_value(float value);

protected:
	void mousePressEvent(QMouseEvent *event) override;

signals:
	void moved(float value);
};

#endif //SLIDER_HPP
