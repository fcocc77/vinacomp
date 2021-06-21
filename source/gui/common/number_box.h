#ifndef NUMBER_BOX_H
#define NUMBER_BOX_H

#include <QContextMenuEvent>
#include <QLineEdit>
#include <QMenu>

class number_box : public QLineEdit
{
    Q_OBJECT
private:
    QMenu *menu;
    QWidget *_knob;
    int dimension;

    float value;
    QString string_value;

    void focusInEvent(QFocusEvent *event) override;
    QString clamp_value(float value);

public:
    number_box(QWidget *_knob, int dimension = 0);
    void set_menu(QMenu *_menu);
    void set_value(float value);
    inline float get_value() const;

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

signals:
    void changed(float value);
};

inline float number_box::get_value() const
{
    return value;
}

#endif // NUMBER_BOX_H
