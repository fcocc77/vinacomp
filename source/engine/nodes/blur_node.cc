#include <blur_node.h>

blur_node::blur_node()
{

}

blur_node::~blur_node()
{

}

void blur_node::render(QImage *image, QJsonObject *params, int frame)
{
	for ( int x = 0; x < 1920; x++)
		for ( int y = 0; y < 1080; y++)
			QColor color = image->pixel(100, 108);
}
