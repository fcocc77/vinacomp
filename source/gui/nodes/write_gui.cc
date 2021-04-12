#include <write_gui.h>
#include <util.h>

write_gui::write_gui() {

    // knob *render_button = get_knob("render");
    // print(render_button->get_name());

}

write_gui::~write_gui() {}

void write_gui::changed(QString param_name)
{
    print(param_name);
}
