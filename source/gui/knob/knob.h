#ifndef KNOB_HPP
#define KNOB_HPP

#include <QHBoxLayout>
#include <QJsonObject>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QWidget>

#include <button.h>
#include <project_struct.h>

class knob : public QWidget
{
    Q_OBJECT
private:
    QHBoxLayout *knob_layout;
    button *animation_button;
    QWidget *_vinacomp;
    QWidget *_parent;

    QJsonObject knob_data;
    QJsonObject *params;

    QList<QWidget *> *viewers_gl;

    void set_keyframe();

protected:
    QString name;
    QString type;
    QString anim_name;

    QMenu *menu;

    project_struct *project;
    bool animated;

    void update_handler();
    void update_value(QJsonValue value);
    inline QJsonValue get_default() const;
    inline QString get_label() const;
    virtual void set_animated(bool _animated);

public:
    knob();
    ~knob();

    void set_init_space(int space, QString label = "");
    QJsonValue get_param_value() const;
    void set_env(QWidget *parent, project_struct *project, QWidget *_vinacomp,
                 QList<QWidget *> *viewers_gl);
    void set_data(QJsonObject _knob_data, QJsonObject *_params);
    QString get_node_type() const;
    QString get_node_name() const;
    inline void set_knob_layout(QHBoxLayout *layout);
    inline QString get_full_name() const;
    inline QString get_type() const;
    inline QString get_name() const;
    inline bool is_animated() const;
    inline void set_visible(bool visible);
    void set_param_value(QJsonValue value);
    void set_animatable(bool _animatable = true);

    virtual void update_animated();
    virtual void restore_param();

    QLabel *label_widget;
    QWidget *init_space;

    int icon_size;
};

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
