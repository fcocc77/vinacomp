#ifndef COLOR_PICKER_H
#define COLOR_PICKER_H

#include <QMouseEvent>
#include <QPainter>
#include <QVBoxLayout>
#include <QWidget>

class color_box : public QWidget
{
    Q_OBJECT
private:
    QColor color;
    QPointF current_position;

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void update_picker();

public:
    void set_color(QColor color);
signals:
    void changed(QColor color);
};

class rainbow_box : public QWidget
{
    Q_OBJECT
private:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void emmit_signal(float pos_x);

signals:
    void changed(QColor color);
};

class color_picker : public QWidget
{
    Q_OBJECT
private:
    QVBoxLayout *layout;

public:
    color_picker();
    ~color_picker();
signals:
    void changed(QColor color);
};

#endif // COLOR_PICKER_H
