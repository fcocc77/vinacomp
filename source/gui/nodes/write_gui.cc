#include <knob_file.h>
#include <knob_intd.h>
#include <knob_floating.h>
#include <util.h>
#include <write_gui.h>
#include <write_node.h>
#include <knob_choice.h>

write_gui::write_gui(project_struct *_project)
{
    _renderer = new renderer(_project);
    connect(_renderer, &renderer::finished_render, this,
            &write_gui::finished_render);
}

write_gui::~write_gui()
{
    delete _renderer;
}

void write_gui::changed(knob *_knob)
{
    QString param_name = _knob->get_name();

    if (param_name == "render")
        render();
    else if (param_name == "reset")
        reset_range();
    else if (param_name == "file_type")
        change_format(_knob);
}

void write_gui::change_format(knob *_file_type)
{
    knob_choice *type_knob = static_cast<knob_choice *>(_file_type);
    QString type = type_knob->get_value().toString();

    get_knob("jpeg_quality")->set_visible(type == "jpeg");
}

void write_gui::start_render()
{
    render_data rdata;
    rdata.width = 1920;
    rdata.height = 1080;
    rdata.root_node = name;
    rdata.frame = opt.frame;
    rdata.layer = "main";

    _renderer->render(rdata);
}

void write_gui::finished_render(render_data _rdata)
{
    if (_rdata.frame > opt.last_frame)
        return;

    QString output = opt.filename;
    output.replace("###", QString::number(_rdata.frame));

    cv::imwrite(output.toStdString(), _rdata.image,
                {cv::IMWRITE_JPEG_QUALITY, opt.jpeg_quality});

    progress_knob->set_value(get_progress());

    opt.frame++;
    start_render();
}

int write_gui::get_progress() const
{
    int count = opt.last_frame - opt.first_frame;
    int current_count = count - (opt.last_frame - opt.frame);

    return (current_count * 100) / count;
}

void write_gui::render()
{
    knob_file *file_knob = static_cast<knob_file *>(get_knob("filename"));
    knob_intd *frame_range_knob =
        static_cast<knob_intd *>(get_knob("frame_range"));
    progress_knob = static_cast<knob_progress *>(get_knob("progress"));

    opt.filename = file_knob->get_param_value().toString();

    opt.first_frame = frame_range_knob->get_value(0);
    opt.last_frame = frame_range_knob->get_value(1);
    opt.frame = opt.first_frame;

    // JPEG Quality
    knob_floating *jq = static_cast<knob_floating *>(get_knob("jpeg_quality"));
    opt.jpeg_quality = jq->get_value() * 100;
    //

    start_render();
}

void write_gui::reset_range()
{
    knob_intd *range_knob = static_cast<knob_intd *>(get_knob("frame_range"));

    range_knob->set_values({_project_settings->get_first_frame(),
                            _project_settings->get_last_frame()});
}
