#ifndef TRIM_PANEL_HPP
#define TRIM_PANEL_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>
#include <QJsonArray>

// VinaComp
#include <trim_panel.h>
#include <properties.h>
#include <qt.h>
#include <tab_widget.h>
#include <nodes_load.h>
#include <knob_editor.h>

#include <knob.h>
#include <knob_color.h>
#include <knob_check_box.h>
#include <knob_file.h>
#include <knob_choice.h>
#include <knob_text.h>
#include <knob_label.h>
#include <knob_button.h>
#include <knob_group.h>
#include <knob_integer.h>
#include <knob_floating.h>
#include <knob_separator.h>
#include <knob_dimensions.h>

class trim_panel : public QWidget
{
private:
	QVBoxLayout *layout;

	knob_editor *_knob_editor;
	nodes_load *nodes_loaded;
	QWidget *knob_editor_container;
	bool knob_editor_visible;
	QPushButton *knob_editor_button;

    tab_widget *tabs;
    bool is_maximize = true;
    properties *_properties;

	QWidget *controls_tab;
    QVBoxLayout *controls_layout;
    QLineEdit *name_edit;

    QString name;
	QString type;
    QString icon_name;

	QJsonObject *data;

	void knob_editor_toggle();
    void setup_ui();
    QWidget *top_buttons_setup_ui();
    tab_widget *tabs_ui();
    void setup_knobs();

public:
    trim_panel(
        properties *_properties,
        QString _name,
		QString _type,
        QString _icon_name,
		nodes_load *nodes_loaded,
		QJsonObject *_data
	);
    ~trim_panel();

    QString get_name();
    void maximize(bool _maximize);
    void set_name(QString _name);
};

#endif //TRIM_PANEL_HPP
