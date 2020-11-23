#ifndef BUTTON_H
#define BUTTON_H

#include <QPushButton>
#include <QString>

#include <qt.h>

class button : public QPushButton
{
private:
    int size_icon;
    QString normal_icon;
    QString hover_icon;
    void change_icon(QString name);

public:
    button(QWidget *parent = 0);
    ~button();

    void set_icon(QString name, int size = 20);
    void set_hover_icon();
    void set_normal_icon();
};

#endif //BUTTON_H