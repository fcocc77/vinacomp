#ifndef READ_H
#define READ_H

#include <QJsonObject>
#include <QImage>

#include <node_engine.h>
#include <util.h>

class read_node : public node_engine
{
private:
	QString frame_to_string(int frame, int frame_digits);
public:
	read_node();
	~read_node();

	// debe retornar un imagen en bits
	QImage *render(QImage *image, QJsonObject *params, int frame) override;
};

#endif // READ_H
