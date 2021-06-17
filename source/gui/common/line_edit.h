#ifndef LINE_EDIT_H
#define LINE_EDIT_H

#include <QContextMenuEvent>
#include <QLineEdit>
#include <QMenu>

class line_edit : public QLineEdit
{
private:
    QMenu *menu;
    QWidget *_knob;
    int dimension;

    void focusInEvent(QFocusEvent *event) override;

public:
    line_edit(QWidget *_knob, int dimension = 0);
    void set_menu(QMenu *_menu);
    void set_clamp_value(float value);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif // LINE_EDIT_H
