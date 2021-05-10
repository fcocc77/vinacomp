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
};

class knob : public QWidget
{
    Q_OBJECT
private:
    QHBoxLayout *knob_layout;
    button *animation_button;
    QWidget *_vinacomp;
    QWidget *panel;
    QGraphicsItem *this_node;
    QWidget *_knob_editor;

    QList<QWidget *> *viewers_gl;
    bool edit_mode;
    button *delete_knob_button;
    button *edit_knob_button;
    button *drag_knob_button;

    void set_keyframe(bool auto_value = true);
    void enable_animation();
    void disable_animation();

protected:
    QString name;
    QString label;
    QString tips;
    QString type;
    QString anim_name;

    QMenu *menu;

    QJsonObject knob_data;
    QJsonObject *params;
    int init_space_width;

    project_struct *project;
    bool animated;

    void update_handler();
    void update_value(QJsonValue value);
    inline QJsonValue get_default() const;
    inline QString get_label() const;
    virtual void set_animated(bool _animated);

public:
    knob(knob_props props = {});
    ~knob();

    void set_edit_mode(bool enable);
    void set_editing_knob(bool editing);
    void set_init_space(int space, QString label = "");
    QJsonValue get_param_value() const;

    void set_data(QJsonObject _knob_data, QJsonObject *_params);

    QString get_node_type() const;
    QString get_node_name() const;
    inline void set_knob_layout(QHBoxLayout *layout);
    inline QString get_full_name() const;
    inline QString get_type() const;
    inline QString get_name() const;
    inline bool is_animated() const;
    inline void set_visible(bool visible);
    inline QWidget *get_panel() const;
    void set_param_value(QJsonValue value);
    void set_animatable(bool _animatable = true);
    inline QJsonObject get_knob_data() const;

    virtual void update_animated();
    virtual void restore_param();

    QLabel *label_widget;
    QWidget *init_space;

    int icon_size;

signals:
    void to_node_gui(knob *_knob);
};

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
    return knob_data.value("label").toString();
}

inline void knob::set_param_value(QJsonValue value)
{
    (*params)[name] = value;
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

inline QString knob::get_full_name() const
{
    // Obtiene el nombre de la ruta completa del parametro
    // 'node_name.param_name'

    return get_node_name() + "." + name;
}

#endif // KNOB_HPP
