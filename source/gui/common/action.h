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
	bool _one_check_at_time;
	QWidget *_tools;
	QString object_name;

	QPushButton *button;
	int icon_size;

	void uncheck_all();

public:
    action(QString label, QString shortcut_key, QString icon_name = "");
    ~action();

    QString get_icon_name() const;
    void set_tool_tip(QString tip);
    QString get_tool_tip() const;
    QString get_label() const;
	void set_checkable(bool checkable = true);
	bool is_checked() const;
	void set_checked(bool _checked);
	QPushButton *make_button(QWidget *_tools, int _icon_size, bool _one_check_at_time);
	void set_object_name(QString name);
	void update();
	void set_visible(bool _visible);
	void set_icon(QString icon_name);

    template <class T, typename Func>
    void connect_to(T *obj, Func func)
    {
        // conecta la funcion a la accion
        connect(this, &QAction::triggered, obj, [=]() {
			bool _checked = checked; // Guardar estado antes de 'uncheck_all'
			uncheck_all();
			set_checked(!_checked);

            func();
        });

        obj->addAction(this);
    }
};

#endif // ACTION_HPP
