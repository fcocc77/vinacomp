#ifndef KNOB_TEXT_HPP
#define KNOB_TEXT_HPP

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>

#include <knob.h>

class knob_text : public knob
{
    Q_OBJECT
private:
    QLineEdit *text;

    void restore_param() override;
public:
    knob_text(knob_props props, QString default_text = "");
    ~knob_text();

    inline void set_text(QString text);
    inline QString get_value() const;
signals:
    void changed(QString text);
    void text_changed(QString text);
};

inline void knob_text::set_text(QString _text)
{
    text->setText(_text);
}

inline QString knob_text::get_value() const
{
    return text->text();
}

#endif // KNOB_TEXT_HPP

