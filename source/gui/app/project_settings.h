#ifndef PROJECT_SETTINGS_H
#define PROJECT_SETTINGS_H

#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

#include <stdio.h>

class project_settings : public QWidget
{
private:
    QVBoxLayout *layout;
    QWidget *_vinacomp;

    int first_frame, last_frame;
    float proxy_scale;
    QTextEdit *comment_text_edit;

public:
    project_settings(QWidget *_vinacomp);
    ~project_settings();

    std::pair<int, int> get_frame_range() const;
};

#endif // PROJECT_SETTINGS_H
