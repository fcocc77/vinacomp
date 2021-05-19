#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

// QT5
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

#include <knob_editor.h>
#include <script_editor.h>

class properties : public QWidget
{
private:
    void setup_ui();
    QWidget *setup_tool_bar();

    QVBoxLayout *trim_panels_layout;
    QWidget *trim_panels;
    QWidget *_vinacomp;

    knob_editor *_knob_editor;
    script_editor *expression_editor;

    bool is_maximize;
    int max_panels;
    bool edit_mode;

    void minimize_all_panels();
    void close_all();
    void limit_panels(int amount);
    void update_viewers_handlers();
    void update_curve(QWidget *_trim_panel);

public:
    properties(QWidget *_vinacomp);
    ~properties();

    void close_trim_panel(QString panel_name);
    QList<QWidget *> get_trim_panels();
    QWidget *get_trim_panel(QString panel_name);
    QWidget *get_trim_panel(int index);
    QWidget *get_first_panel() ;

    template <class T> void add_trim_panel(T *_trim_panel);

    void update_animated_knobs();
    void set_edit_mode(bool enable);
    inline QWidget *get_knob_editor() const;
    inline bool is_edit_mode() const;
    inline script_editor *get_expression_editor() const;
};
//
//

// Implementacion de Plantillas
template <class T> void properties::add_trim_panel(T *_trim_panel)
{
    limit_panels(max_panels - 1);

    trim_panels_layout->addWidget(_trim_panel);
    _trim_panel->enter_to_properties();

    update_curve(_trim_panel);
    update_viewers_handlers();
}

inline script_editor *properties::get_expression_editor() const
{
    return expression_editor;
}

inline bool properties::is_edit_mode() const
{
    return edit_mode;
}

inline QWidget *properties::get_knob_editor() const
{
    return _knob_editor;
}

#endif // PROPERTIES_HPP
