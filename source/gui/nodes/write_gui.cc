#include <knob_file.h>
#include <knob_intd.h>
#include <util.h>
#include <write_gui.h>
#include <write_node.h>

write_gui::write_gui(project_struct *_project)
    : first_frame(1)
    , last_frame(100)
    , frame(1)
{
    _renderer = new renderer(_project);
    connect(_renderer, &renderer::finished_render, this,
            &write_gui::finished_render);
}

write_gui::~write_gui()
{
    delete _renderer;
}

void write_gui::start_render()
{
    render_data rdata;
    rdata.width = 1920;
    rdata.height = 1080;
    rdata.root_node = name;
    rdata.frame = frame;
    rdata.layer = "main";

    _renderer->render(rdata);
}

void write_gui::finished_render(render_data _rdata)
{
    if (_rdata.frame > last_frame)
        return;

    QString output = filename;
    output.replace("###", QString::number(_rdata.frame));

    cv::imwrite(output.toStdString(), _rdata.image);

    progress_knob->set_value(get_progress());

    frame++;
    start_render();
}

int write_gui::get_progress() const
{
    int count = last_frame - first_frame;
    int current_count = count - (last_frame - frame);

    return (current_count * 100) / count;
}

void write_gui::render()
{
    knob_file *file_knob = static_cast<knob_file *>(get_knob("filename"));
    knob_intd *frame_range_knob =
        static_cast<knob_intd *>(get_knob("frame_range"));
    progress_knob = static_cast<knob_progress *>(get_knob("progress"));

    filename = file_knob->get_param_value().toString();

    first_frame = frame_range_knob->get_value(0);
    last_frame = frame_range_knob->get_value(1);
    frame = first_frame;

    start_render();
}

void write_gui::reset_range()
{
    knob_intd *range_knob = static_cast<knob_intd *>(get_knob("frame_range"));

    range_knob->set_values({_project_settings->get_first_frame(),
                            _project_settings->get_last_frame()});
}

void write_gui::changed(QString param_name)
{
    if (param_name == "render")
        render();
    else if (param_name == "reset")
        reset_range();
}
