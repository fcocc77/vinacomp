#ifndef WRITE_PANEL_H
#define WRITE_PANEL_H

#include <knob_progress.h>
#include <node_panel.h>
#include <render_data.h>
#include <renderer.h>

// Engine
#include <run_render.h>

class write_panel : public node_panel
{
private:
    void changed(knob *_knob) override;
    renderer *_renderer;

    knob_progress *progress_knob;
    render_options opt;

    void start_render();
    void finished_render(render_data _rdata);
    int get_progress() const;

    void render();
    void reset_range();
    void change_format(knob *_file_type);

public:
    write_panel(project_struct *project);
    ~write_panel();
};

#endif // WRITE_PANEL_H
