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

public:
    line_edit(QWidget *_knob);
    void set_menu(QMenu *_menu);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif // LINE_EDIT_H
