#ifndef RENDERER_H
#define RENDERER_H

#include <QString>
#include <QWidget>

#include <util.h>
#include <project_struct.h>
#include <node_engine.h>
#include <read_node.h>
#include <blur_node.h>
#include <viewer_node.h>
#include <time_offset_node.h>
#include <frame_range_node.h>
#include <position_node.h>
#include <invert_node.h>
#include <reformat_node.h>
#include <constant_node.h>
#include <ramp_node.h>
#include <radial_node.h>
#include <noise_node.h>
#include <shuffle_node.h>
#include <grid_node.h>
#include <checker_board_node.h>
#include <text_node.h>
#include <mirror_node.h>
#include <sharpen_node.h>
#include <cornel_pin_node.h>
#include <rectangle_node.h>
#include <frame_hold_node.h>
#include <copy_node.h>
#include <clamp_node.h>
#include <saturation_node.h>
#include <grade_node.h>
#include <edge_detect_node.h>
#include <erode_node.h>
#include <keyer_node.h>
#include <dissolve_node.h>
#include <merge_node.h>
#include <premult_node.h>
#include <unpremult_node.h>
#include <switch_node.h>
#include <bbox_adjust_node.h>
#include <crop_node.h>
#include <idistort_node.h>
#include <lens_distortion_node.h>
#include <transform_node.h>

class renderer : public QWidget
{
private:
	project_struct *project;

	QMap <QString, node_engine*> nodes;
	time_offset_node *time_offset;
	read_node *read;
	frame_range_node *_frame_range;

public:
	renderer(project_struct *_project);
	~renderer();

	void render(
		QImage *image,
		int frame,
		QString node_name,
		pair <int, int> &frame_range,
		QRect &bbox
	);

	pair <int, int> get_frame_range(QString node_name) const;
	QString get_input_node(QString node_name, int input = 0) const;
};

#endif // RENDERER_H
