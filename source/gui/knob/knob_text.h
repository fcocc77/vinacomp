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
    knob_text(QString default_text = "");
    ~knob_text();

    inline void set_text(QString text);
signals:
    void changed(QString text);
    void text_changed(QString text);
};

inline void knob_text::set_text(QString _text)
{
    text->setText(_text);
}

#endif // KNOB_TEXT_HPP
