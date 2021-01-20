#include <project_settings.h>
#include <vinacomp.h>

project_settings::project_settings(
		QWidget *__vinacomp
	)
	: _vinacomp(__vinacomp)
	, first_frame(0)
	, last_frame(100)
	, proxy_scale(1)
{
	this->hide();
	this->setMinimumWidth(500);
	this->setMaximumWidth(500);
	this->setObjectName("project_settings");

	layout = new QVBoxLayout(this);
	layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

	int init_space = 140;

	// Frame Range
	QList <int> default_value = {1, 100};
	knob_dimensions *frame_range_knob = new knob_dimensions(default_value);
	connect(frame_range_knob, &knob_dimensions::changed_int, this, [=](QList <int> values){
		auto *viewers = dynamic_cast<vinacomp *>(_vinacomp)->get_viewers();
		for ( viewer *_viewer : *viewers )
			_viewer->set_frame_range(values[0], values[1]);
	});
	frame_range_knob->set_init_space(init_space, "Frame Range");
	//
	//

	// Proxy Scale
	knob_floating *proxy_scale_knob = new knob_floating(0.1, 1, proxy_scale);
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
