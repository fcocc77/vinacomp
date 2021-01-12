#ifndef ACTION_HPP
#define ACTION_HPP

#include <QAction>
#include <QPushButton>

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
	bool visible;

	QPushButton *button;
	int icon_size;

public:
    action(QString label, QString shortcut_key, QString icon_name = "");
    ~action();

    QString get_icon_name() const;
    void set_tool_tip(QString tip);
    QString get_tool_tip() const;
    QString get_label() const;
	void set_checkable(bool checkable);
	bool is_checked() const;
	void set_checked(bool _checked);
	QPushButton *make_button(QWidget *_tools, int _icon_size, bool uncheck_all);
	void update();
	void set_visible(bool _visible);
	void set_icon(QString icon_name);

    template <class T, typename Func>
    void connect_to(T *obj, Func func)
    {
        // conecta la funcion a la accion
        connect(this, &QAction::triggered, obj, [=]() {
            func();
        });

        obj->addAction(this);
    }
};

#endif // ACTION_HPP
