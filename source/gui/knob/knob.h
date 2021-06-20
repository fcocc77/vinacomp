#ifndef KNOB_HPP
#define KNOB_HPP

#include <QHBoxLayout>
#include <QJsonObject>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QWidget>
#include <QGraphicsItem>

#include <button.h>
#include <project_struct.h>

struct knob_props
{
    QWidget *panel;
    project_struct *project;
    QWidget *_vinacomp;
    QList<QWidget *> *viewers_gl;
    QGraphicsItem *this_node;
    QWidget *_knob_editor;
    QJsonObject knob_data;
    QJsonObject *params;
};

class knob : public QWidget
{
    Q_OBJECT
private:
    QHBoxLayout *knob_layout;
    button *animation_button;
    QWidget *panel;
    QGraphicsItem *this_node;
    QWidget *_knob_editor;

    QList<QWidget *> *viewers_gl;
    bool edit_mode;
    button *delete_knob_button;
    button *edit_knob_button;
    button *drag_knob_button;

    bool linked;
    QString handler_knob_node_name, handler_knob_name;

    void update_knob_in_curve_editor();

protected:
    QWidget *_vinacomp;

    QString name, label, tips, type;
    QString curve_name, exp_name, handler_name, slaves_name;
    QWidget *over_line_widget;

    QJsonArray slaves_knobs;

    QJsonObject knob_data;
    QJsonObject *params;
    int init_space_width;

    project_struct *project;
    bool animated;

    void update_handler();
    void update_value(QJsonValue value);
    inline QJsonValue get_default() const;
    virtual void set_has_expression(bool expression);

public:
    knob(knob_props props = {});
    ~knob();

    virtual void set_error(bool error);
    virtual void set_animated(bool _animated, int dimension = -1);
    virtual void set_disable(bool disable);
    void add_slave_knob(QString node_name, QString param_name);
    void remove_slave_knob(QString node_name, QString param_name);
    QList<knob *> get_slaves_knobs() const;
    knob *get_knob(QString node_name, QString param_name) const;
    void set_linked_handler(QString node_name, QString param_name);
    void remove_link();
    void remove_link(QString node_name);
    inline void rename_handler_node_name(QString handler_name,
                                         QString new_handler_name);
    void restore_slaves_konbs();
    void enable_animation();
    void disable_animation(int dimension = -1);
    void update_keyframe(float value, int dimension, bool force = false);
    void set_keyframe(int dimension = -1);

    void set_edit_mode(bool enable);
    void set_editing_knob(bool editing);
    void set_init_space(int space);
    inline void set_over_line_widget(QWidget *widget);
    inline QWidget *get_over_line_widget() const;
    QJsonValue get_param_value() const;

    virtual void restore_default();
    void set_data();
    QString get_node_type() const;
    QString get_node_name() const;
    void set_expression(QString expression);
    inline QString get_expression() const;
    void set_init_label(bool has_label);
    void set_init_label_text(QString label);
    inline void set_knob_layout(QHBoxLayout *layout);
    inline QString get_full_name() const;
    inline QString get_type() const;
    inline QString get_name() const;
    inline QString get_tips() const;
    inline QString get_label() const;
    inline QString get_curve(int dimension = 0) const;
    inline void set_curve(QString curve, int dimension = 0);
    inline bool is_animated() const;
    inline void set_visible(bool visible);
    inline QWidget *get_panel() const;
    void set_param_value(QJsonValue value);
    void set_animatable(bool _animatable = true);
    inline QJsonObject get_knob_data() const;
    inline int get_init_space_width() const;
    QWidget *get_node_view() const;

    virtual void update_animated();
    virtual void restore_param();

    QLabel *label_widget;
    QWidget *init_space;

    int icon_size;

signals:
    void to_node_panel(knob *_knob);
};

inline void knob::rename_handler_node_name(QString handler_name,
                                           QString new_handler_name)
{
    if (handler_knob_node_name == handler_name)
        handler_knob_node_name = new_handler_name;
}

inline QString knob::get_expression() const
{
    return params->value(exp_name).toString();
}

inline int knob::get_init_space_width() const
{
    return init_space_width;
}

inline void knob::set_over_line_widget(QWidget *widget)
{
    over_line_widget = widget;
}

inline QWidget *knob::get_over_line_widget() const
{
    return over_line_widget;
}

inline QJsonObject knob::get_knob_data() const
{
    return knob_data;
}

inline QWidget *knob::get_panel() const
{
    return panel;
}

inline void knob::set_visible(bool visible)
{
    this->setVisible(visible);
}

inline QJsonValue knob::get_default() const
{
    return knob_data.value("default");
}

inline QString knob::get_label() const
{
    return label;
}

inline void knob::set_param_value(QJsonValue value)
{
    (*params)[name] = value;
}

inline QString knob::get_curve(int dimension) const
{
    return params->value(curve_name).toArray()[dimension].toString();
}

inline void knob::set_curve(QString curve, int dimension)
{
    QJsonArray curves = params->value(curve_name).toArray();
    curves[dimension] = curve;
    (*params)[curve_name] = curves;
}

inline bool knob::is_animated() const
{
    return animated;
}

inline void knob::set_knob_layout(QHBoxLayout *layout)
{
    // es el layout del hijo que heredo esta clase, para poder agregarle
    // el boton de animacion final.
    knob_layout = layout;
}

inline QString knob::get_type() const
{
    return type;
}

inline QString knob::get_name() const
{
    return name;
}

inline QString knob::get_tips() const
{
    return tips;
}

inline QString knob::get_full_name() const
{
    // Obtiene el nombre de la ruta completa del parametro
    // 'node_name.param_name'

    return get_node_name() + "." + name;
}

#endif // KNOB_HPP
