#ifndef WRITE_GUI_H
#define WRITE_GUI_H

#include <knob_progress.h>
#include <node_gui.h>
#include <render_data.h>
#include <renderer.h>

class write_gui : public node_gui
{
private:
    void changed(knob *_knob) override;
    renderer *_renderer;

    knob_progress *progress_knob;
    int first_frame, last_frame, frame;
    QString filename;

    void start_render();
    void finished_render(render_data _rdata);
    int get_progress() const;

    void render();
    void reset_range();
    void change_format(knob *_file_type);

public:
    write_gui(project_struct *project);
    ~write_gui();
};

#endif // WRITE_GUI_H
