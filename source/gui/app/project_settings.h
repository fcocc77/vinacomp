#ifndef PROJECT_SETTINGS_H
#define PROJECT_SETTINGS_H

#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

#include <knob_floating.h>
#include <knob_intd.h>
#include <viewer.h>

class project_settings : public QWidget
{
private:
    QVBoxLayout *layout;
    QWidget *_vinacomp;

    int first_frame, last_frame;
    float proxy_scale;
    QTextEdit *comment_text_edit;

public:
    project_settings( QWidget *_vinacomp );
    ~project_settings();

    pair<int, int> get_frame_range() const;
};

#endif // PROJECT_SETTINGS_H
