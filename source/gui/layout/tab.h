#ifndef TAB_H
#define TAB_H

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStyle>
#include <QWidget>

#include <button.h>
#include <qt.h>

class tab : public QWidget
{
private:
    bool _checked;
    QWidget *_tab_widget;
    const bool has_close_button;
    bool visible_close_button;

    QString name;
    QWidget *content;
    QLabel *label;
    button *close_button;
    button *edit_button;
    QHBoxLayout *layout;
    QWidget *_knob_editor;
    QWidget *panel;

    void set_margin_with_buttons();

public:
    tab(QWidget *_tab_widget, QString name, QWidget *_content,
        bool _has_close_button = false, QWidget *_knob_editor = nullptr,
        QWidget *panel = nullptr);
    ~tab();

    inline QWidget *get_widget() const;
    inline bool checked() const;
    void set_checked(bool __checked);
    inline QString get_name() const;
    inline QWidget *get_panel() const;
    void set_name(QString name);

    void set_visible_close_button(bool visible);
    void set_visible_edit_button(bool visible);

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

inline QWidget *tab::get_panel() const
{
    return panel;
}

inline QString tab::get_name() const
{
    return name;
}

inline QWidget *tab::get_widget() const
{
    return content;
}

inline bool tab::checked() const
{
    return _checked;
}


#endif // TAB_H
