#ifndef ACTION_HPP
#define ACTION_HPP

#include <QAction>

#include <util.h>
#include <qt.h>

class action : public QAction
{
private:
    QString key;
    QString icon_name;
    QString tool_tip;
    QString label;
	bool checkable;
	bool checked;

public:
    action(QString label, QString shortcut_key, QString icon_name = "");
    ~action();

    QString get_icon_name() const;
    void set_tool_tip(QString tip);
    QString get_tool_tip() const;
    QString get_label() const;
	void set_checkable(bool checkable);
	bool is_checked() const;

    template <class T, typename Func>
    void connect_to(T *obj, Func func)
    {
        // conecta la funcion a la accion
        connect(this, &QAction::triggered, obj, [=]() {
            func();
			if (checkable)
				checked = !checked;
        });

        obj->addAction(this);
    }
};

#endif // ACTION_HPP
