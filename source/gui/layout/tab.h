#ifndef TAB_H
#define TAB_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QStyle>

#include <qt.h>
#include <button.h>

class tab : public QWidget
{
private:
    bool _checked;
    QWidget *_tab_widget;
    const bool has_close_button;

    QString name;
    QWidget *content;
    QLabel *label;
    button *close_button;
    QHBoxLayout *layout;

public:
    tab(QWidget *_tab_widget,
        QString name,
        QWidget *_content,
        bool _has_close_button = false);
    ~tab();

    QWidget *get_content_widget() const;
    bool checked() const;
    void set_checked(bool __checked);
    QString get_name() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // TAB_H