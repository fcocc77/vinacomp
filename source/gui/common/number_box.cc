#include <QTimer>
#include <QDoubleValidator>

#include <knob.h>
#include <knob_curve_menu.h>
#include <number_box.h>
#include <util.h>

number_box::number_box(QWidget *__knob, int _dimension)
    : menu(nullptr)
    , _knob(__knob)
    , dimension(_dimension)
    , value(0)
{
    // permite solo numeros
    QDoubleValidator *validator = new QDoubleValidator();
    this->setValidator(validator);

    connect(this, &QLineEdit::editingFinished, this, [=]() {
        if (this->text() == string_value)
            return;

        value = this->text().toDouble();
        changed(value);
    });
}

void number_box::set_menu(QMenu *_menu)
{
    if (!menu)
        menu = _menu;
}

void number_box::contextMenuEvent(QContextMenuEvent *event)
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

void number_box::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    QTimer::singleShot(0, this, &QLineEdit::selectAll);
}

QString number_box::clamp_value(float value)
{
    QStringList _value = QString::number(value).split('.');

    QString number = _value.first();
    QString decimal;

    if (_value.count() == 2)
        decimal = _value.last() + "00";
    else
        decimal = "00";

    return (number + '.' + decimal.left(2));
}

void number_box::set_value(float _value)
{
    value = _value;
    string_value = clamp_value(value);
    this->setText(string_value);
}

