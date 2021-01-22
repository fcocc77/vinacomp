#ifndef FRAME_RANGE_GUI_H
#define FRAME_RANGE_GUI_H

#include <node_gui.h>
#include <knob_dimensions.h>

class frame_range_gui : public node_gui
{
public:
	frame_range_gui();
	~frame_range_gui();

	void clicked() override;
};

#endif // FRAME_RANGE_GUI_H
