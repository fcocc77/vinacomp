#ifndef READ_H
#define READ_H

#include <QJsonObject>
#include <QImage>

#include <util.h>

class read_node
{
private:
	QString frame_to_string(int frame, int frame_digits);
public:
	read_node();
	~read_node();

	// debe retornar un imagen en bits
	QImage render(QJsonObject *params, int frame);
};

#endif // READ_H
