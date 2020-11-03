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
#include <trim_panel.hpp>
#include <properties.hpp>
#include <qt.hpp>

#include <knob_color.hpp>
#include <knob_check_box.hpp>
#include <knob_file.hpp>
#include <knob_choice.hpp>
#include <knob_text.hpp>
#include <knob_label.hpp>
#include <knob_button.hpp>
#include <knob_group.hpp>
#include <knob_integer.hpp>
#include <knob_floating.hpp>
#include <knob_separator.hpp>
#include <knob_position.hpp>

class trim_panel : public QWidget
{
private:
    QTabWidget *tabs;
    bool is_maximize = true;
    properties *_properties;

    QVBoxLayout *controls_layout;

    QString name;
    QString icon_name;

    void setup_ui();
    QWidget *top_buttons_setup_ui();
    QTabWidget *tabs_ui();

public:
    trim_panel(
        properties *_properties,
        QString _name,
        QString _icon_name,
        QJsonArray *_knobs);
    ~trim_panel();

    QString get_name();
    void setup_knobs(QJsonArray *knobs);
    void maximize(bool _maximize);
};

#endif //TRIM_PANEL_HPP