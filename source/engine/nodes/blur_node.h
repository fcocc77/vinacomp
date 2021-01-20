#ifndef BLUR_NODE_H
#define BLUR_NODE_H

#include <QImage>

#include <util.h>

class blur_node
{
public:
	blur_node();
	~blur_node();

	QImage render(QImage &image);
};

#endif // BLUR_NODE_H
