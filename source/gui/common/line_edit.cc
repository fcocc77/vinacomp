#include <QTimer>

#include <knob.h>
#include <knob_curve_menu.h>
#include <line_edit.h>
#include <util.h>

line_edit::line_edit(QWidget *__knob)
    : menu(nullptr)
    , _knob(__knob)
{
}

void line_edit::set_menu(QMenu *_menu)
{
    if (!menu)
        menu = _menu;
}

void line_edit::contextMenuEvent(QContextMenuEvent *event)
{
    QPoint global = this->mapToGlobal(event->pos());

    if (menu)
    {
        static_cast<knob_curve_menu *>(menu)->set_knob(
            static_cast<knob *>(_knob));

        menu->move(global);
        menu->show();
    }
    else
        QLineEdit::contextMenuEvent(event);
}

void line_edit::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    QTimer::singleShot(0, this, &QLineEdit::selectAll);
}
