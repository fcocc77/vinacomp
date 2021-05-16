#include <knob_floating.h>
#include <knob_intd.h>
#include <project_settings.h>
#include <vinacomp.h>

project_settings::project_settings(QWidget *__vinacomp)
    : _vinacomp(__vinacomp)
    , first_frame(1)
    , last_frame(100)
    , proxy_scale(1)
{

    setup_sequence();
    setup_layers();

    set_content("Sequence");
}

project_settings::~project_settings() {}

void project_settings::setup_sequence()
{
    QVBoxLayout *layout = add_item("Sequence");

    int init_space = 100;

    // Frame Range
    knob_intd *frame_range_knob =
        new knob_intd({}, 2, {first_frame, last_frame});

    connect(
        frame_range_knob, &knob_intd::changed, this, [=](QList<int> values) {
            first_frame = values[0];
            last_frame = values[1];

            auto *viewers = static_cast<vinacomp *>(_vinacomp)->get_viewers();
            for (viewer *_viewer : *viewers)
                _viewer->update_input_range();
        });
    frame_range_knob->set_init_space(init_space);
    frame_range_knob->set_init_label_text("Frame Range");
    //
    //

    // Proxy Scale
    knob_floating *proxy_scale_knob =
        new knob_floating({}, 0.1, 1, proxy_scale);
    proxy_scale_knob->set_init_space(init_space);
    proxy_scale_knob->set_init_label_text("Proxy Scale");
    //

    // Comment
    comment_text_edit = new QTextEdit();
    comment_text_edit->setPlaceholderText("Project Comment");
    //

    layout->addWidget(frame_range_knob);
    layout->addWidget(proxy_scale_knob);
    layout->addWidget(comment_text_edit);
}

void project_settings::setup_layers()
{
    QVBoxLayout *layout = add_item("Layers");
}
