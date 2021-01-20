#ifndef RENDERER_H
#define RENDERER_H

#include <QString>

#include <util.h>
#include <project_struct.h>
#include <read_node.h>
#include <blur_node.h>

class renderer
{
private:
	project_struct *project;
	read_node *read;
	blur_node *blur;

public:
	renderer(project_struct *_project);
	~renderer();

	QImage render(int frame, QString node_name);
};

#endif // RENDERER_H
