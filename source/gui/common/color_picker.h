#ifndef COLOR_PICKER_H
#define COLOR_PICKER_H

#include <QMouseEvent>
#include <QPainter>
#include <QVBoxLayout>
#include <QWidget>

class color_picker : public QWidget
{
    Q_OBJECT
private:
    QVBoxLayout *layout;

    QColor color;
    QPointF current_position;

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void update_picker();

public:
    color_picker();
    ~color_picker();

    static QColor hsl_to_rgb(float H, float S, float L);

    void set_color(QColor color);
    void set_hsl(float hue, float sat = 1, float level = 1);
signals:
    void changed(QColor color);
};

#endif // COLOR_PICKER_H
