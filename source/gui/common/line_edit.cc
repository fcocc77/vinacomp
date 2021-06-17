#include <QTimer>

#include <knob.h>
#include <knob_curve_menu.h>
#include <line_edit.h>
#include <util.h>

line_edit::line_edit(QWidget *__knob, int _dimension)
    : menu(nullptr)
    , _knob(__knob)
    , dimension(_dimension)
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
            static_cast<knob *>(_knob), dimension);

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

void line_edit::set_clamp_value(float value)
{
    QStringList _value = QString::number(value).split('.');

    QString number = _value.first();
    QString decimal;

    if (_value.count() == 2)
        decimal = _value.last() + "00";
    else
        decimal = "00";

    this->setText(number + '.' + decimal.left(2));
}
