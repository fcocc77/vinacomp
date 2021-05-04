#ifndef BUTTON_H
#define BUTTON_H

#include <QPushButton>
#include <QString>

class button : public QPushButton
{
    Q_OBJECT
private:
    int size_icon;
    QString normal_icon;
    QString hover_icon;
    bool hover;

    bool checkable;
    bool checked;

    void change_icon(QString name);

public:
    button(QWidget *parent = 0, bool _hover = false);
    ~button();

    inline void set_checkable(bool checkable = true);
    void set_icon(QString name, int size = 20);
    void set_hover_icon();
    void set_normal_icon();
signals:
    void clicked(bool checked);
};

inline void button::set_checkable(bool _checkable)
{
    checkable = _checkable;
}

#endif // BUTTON_H
