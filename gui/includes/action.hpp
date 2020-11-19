#ifndef ACTION_HPP
#define ACTION_HPP

#include <QAction>

#include <util.hpp>
#include <qt.hpp>

class action : public QAction
{
private:
    QString key;
    QShortcut *_shortcut;

public:
    action(QString label, QString shortcut_key, QString icon_name)
        : key(shortcut_key)
    {
        this->setText(label);

        this->setIcon(QIcon("resources/images/" + icon_name + ".png"));
        this->setShortcut(QString(key));
    }
    ~action()
    {
        delete _shortcut;
    }

    template <class T, typename Func>
    void connect_to(T *obj, Func func)
    {
        // conecta la funcion a la accion
        connect(this, &QAction::triggered, obj, [=]() {
            func();
        });

        // genera shortcut llamando la funcion
        _shortcut = new QShortcut(QKeySequence(key), obj);
        QObject::connect(_shortcut, &QShortcut::activated, obj, func);
    }
};

#endif // ACTION_HPP
