#ifndef KNOB_HPP
#define KNOB_HPP

#include <QHBoxLayout>
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

    QList<QWidget *> *viewers_gl;

protected:
    QString node_name;
    QString node_type;
    QString name;
    QString type;

    QMenu *menu;

    project_struct *project;
    bool animated;

    void update_handler();

public:
    knob();
    ~knob();

    void set_init_space(int space, QString label = "");
    void set_animatable(bool _animatable = true);
    void set_names(QString node_name, QString _node_type, QString _param_name,
                   QString _param_type);
    inline void set_project(project_struct *project);
    inline void set_knob_layout(QHBoxLayout *layout);
    inline void set_visible(bool visible);
    inline void set_viewers_gl(QList<QWidget *> *viewers_gl);
    inline QString get_full_name() const;
    inline QString get_node_type() const;
    inline QString get_node_name() const;
    inline QString get_type() const;
    inline QString get_name() const;
    inline bool is_animated() const;

    virtual inline void set_animated(bool _animated);
    virtual void update_animated();

    QLabel *label_widget;
    QWidget *init_space;

    int icon_size;

signals:
    void key_frame_changed(bool add);
};

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
