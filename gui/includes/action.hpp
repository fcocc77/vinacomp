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
    QString icon_name;

public:
    action(QString label, QString shortcut_key, QString icon_name);
    ~action();

    QString get_icon_name();

    template <class T, typename Func>
    void connect_to(T *obj, Func func)
    {
        // conecta la funcion a la accion
        connect(this, &QAction::triggered, obj, [=]() {
            func();
        });

        // ShortCut, y solo funciona si el puntero esta sobre el widget
        _shortcut = new QShortcut(QKeySequence(key), obj);
        QObject::connect(_shortcut, &QShortcut::activated, obj, [=]() {
            if (obj->underMouse())
                func();
        });
    }
};

#endif // ACTION_HPP
