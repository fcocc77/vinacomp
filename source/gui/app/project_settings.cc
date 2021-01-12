#include <project_settings.h>


project_settings::project_settings()
	: first_frame(0)
	, last_frame(100)
	, proxy_scale(1)
{
	this->hide();
	this->setMinimumWidth(500);
	this->setMaximumWidth(500);

	layout = new QVBoxLayout(this);
	layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

	int init_space = 140;

	// Frame Range
	knob_position *frame_range_knob = new knob_position();
	frame_range_knob->set_init_space(init_space, "Frame Range");
	//
	//
	
	// Proxy Scale
	knob_floating *proxy_scale_knob = new knob_floating(proxy_scale);
	proxy_scale_knob->set_init_space(init_space, "Proxy Scale");
	//

	// Comment
	comment_text_edit = new QTextEdit();
	//


	layout->addWidget(frame_range_knob);
	layout->addWidget(proxy_scale_knob);
	layout->addWidget(comment_text_edit);
}

project_settings::~project_settings()
{

}
