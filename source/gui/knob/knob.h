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
    QString node_name;
    QString node_type;
    QString name;
    QString type;

    QMenu *menu;

    project_struct *project;
    bool animated;

    void update_handler();
    inline QJsonValue get_default() const;
    void update_value(QJsonValue value);

public:
    knob();
    ~knob();

    void set_init_space(int space, QString label = "");
    void set_animatable(bool _animatable = true);
    void set_names(QString node_name, QString _node_type, QString _param_name,
                   QString _param_type);
    QJsonValue get_param_value() const;
    inline void set_project(project_struct *project);
    inline void set_vinacomp(QWidget *_vinacomp);
    inline void set_parent(QWidget *parent);
    inline void set_knob_layout(QHBoxLayout *layout);
    inline void set_knob_data(QJsonObject knob_data);
    inline void set_visible(bool visible);
    inline void set_viewers_gl(QList<QWidget *> *viewers_gl);
    inline QString get_full_name() const;
    inline QString get_node_type() const;
    inline QString get_node_name() const;
    inline QString get_type() const;
    inline QString get_name() const;
    inline bool is_animated() const;
    inline void set_params(QJsonObject *params);
    void set_param_value(QString curve);

    virtual void restore_param();
    virtual inline void set_animated(bool _animated);
    virtual void update_animated();

    QLabel *label_widget;
    QWidget *init_space;

    int icon_size;

signals:
    void key_frame_changed(bool add);
};

inline void knob::set_parent(QWidget *__parent)
{
    _parent = __parent;
}

inline void knob::set_vinacomp(QWidget *__vinacomp)
{
    _vinacomp = __vinacomp;
}

inline QJsonValue knob::get_default() const
{
    return knob_data.value("default");
}

inline void knob::set_knob_data(QJsonObject _knob_data)
{
    knob_data = _knob_data;
}

inline void knob::set_params(QJsonObject *_params)
{
    params = _params;
}

inline void knob::set_param_value(QString curve)
{
    (*params)[name] = curve;
}

inline void knob::set_project(project_struct *_project)
{
    project = _project;
}

inline void knob::set_animated(bool _animated)
{
    animated = _animated;
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

inline void knob::set_visible(bool visible)
{
    this->setVisible(visible);
}

inline void knob::set_viewers_gl(QList<QWidget *> *_viewers_gl)
{
    viewers_gl = _viewers_gl;
}

inline QString knob::get_node_type() const
{
    return node_type;
}

inline QString knob::get_type() const
{
    return type;
}

inline QString knob::get_name() const
{
    return name;
}

inline QString knob::get_node_name() const
{
    return node_name;
}

inline QString knob::get_full_name() const
{
    // Obtiene el nombre de la ruta completa del parametro
    // 'node_name.param_name'

    return node_name + "." + name;
}
#endif // KNOB_HPP
