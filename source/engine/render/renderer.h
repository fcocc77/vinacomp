#ifndef RENDERER_H
#define RENDERER_H

#include <QString>

#include <util.h>
#include <project_struct.h>
#include <node_engine.h>
#include <read_node.h>
#include <blur_node.h>
#include <viewer_node.h>
#include <time_offset_node.h>
#include <frame_range_node.h>

class renderer
{
private:
	project_struct *project;

	QMap <QString, node_engine*> nodes;
	time_offset_node *time_offset;

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
};

#endif // RENDERER_H
