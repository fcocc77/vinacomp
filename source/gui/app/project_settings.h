#ifndef PROJECT_SETTINGS_H
#define PROJECT_SETTINGS_H

#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

#include <settings.h>
#include <stdio.h>

class project_settings : public settings
{
private:
    QWidget *_vinacomp;

    int first_frame, last_frame;
    float proxy_scale;
    QTextEdit *comment_text_edit;

    void setup_sequence();
    void setup_layers();

public:
    project_settings(QWidget *_vinacomp);
    ~project_settings();

    inline std::pair<int, int> get_frame_range() const;
    inline int get_first_frame() const;
    inline int get_last_frame() const;
};

inline std::pair<int, int> project_settings::get_frame_range() const
{
    return {first_frame, last_frame};
}

inline int project_settings::get_first_frame() const
{
    return first_frame;
}

inline int project_settings::get_last_frame() const
{
    return last_frame;
}

#endif // PROJECT_SETTINGS_H
