#include <line_edit.h>
#include <util.h>

line_edit::line_edit()
    : menu(nullptr)
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
        menu->move(global);
        menu->show();
    }
    else
        QLineEdit::contextMenuEvent(event);
}
