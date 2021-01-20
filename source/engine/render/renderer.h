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

class renderer
{
private:
	project_struct *project;

	QMap <QString, node_engine*> nodes;
	time_offset_node *time_offset;

	QImage *image;

public:
	renderer(project_struct *_project);
	~renderer();

	QImage *render(int frame, QString node_name);
};

#endif // RENDERER_H
