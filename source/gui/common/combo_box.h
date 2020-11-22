#ifndef COMBO_BOX_HPP
#define COMBO_BOX_HPP

#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>

#include <util.h>

class combo_box : public QWidget
{
private:
    QLabel *label;
    QWidget *menu;
    QVBoxLayout *menu_layout;

public:
    combo_box(/* args */);
    ~combo_box();

    void add_item(QString name);
    void add_items(QStringList list);

protected:
    void mousePressEvent(QMouseEvent *event);
    void leaveEvent(QEvent *event);
};

#endif // COMBO_BOX_HPP