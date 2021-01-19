#ifndef READ_H
#define READ_H

#include <QJsonObject>
#include <QImage>

#include <util.h>

class read_node
{
public:
	read_node();
	~read_node();

	// debe retornar un imagen en bits
	QImage render(QJsonObject *params);
};

#endif // READ_H
