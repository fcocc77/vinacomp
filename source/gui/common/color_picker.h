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

    float hue, sat, level;
    QColor color;
    QPointF circle_position;

    void update_picker();
    void set_circle_position(float sat, float level);

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

public:
    color_picker();
    ~color_picker();

    static QColor hsl_to_rgb(float H, float S, float L);
    static void rgb_to_hsl(QColor rgb, float &H, float &S, float &L);

    void set_color(QColor color);
    void set_hsl(float hue, float sat = 1, float level = 1);
signals:
    void changed(QColor color, float hue, float sat, float level);
};

#endif // COLOR_PICKER_H
