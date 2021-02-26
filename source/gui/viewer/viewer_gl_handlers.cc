#include <viewer_gl.h>
#include <properties.h>

void viewer_gl::handlers_update()
{
	auto panels = dynamic_cast< properties * >(_properties)->get_trim_panels();
	print(panels);
}


void viewer_gl::box_handler_changed(QRect box, QString name)
{
}

