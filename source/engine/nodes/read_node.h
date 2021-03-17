#ifndef READ_H
#define READ_H

#include <QImage>
#include <QJsonObject>

#include <node_engine.h>
#include <util.h>

class read_node : public node_engine
{
private:
    QString frame_to_string(int frame, int frame_digits);

public:
    read_node();
    ~read_node();

    void render(render_data *rdata, QJsonObject *params) override;

    pair<int, int> get_frame_range(QJsonObject *params) const;
};

#endif // READ_H
