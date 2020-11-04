#ifndef INIT_SPACE_HPP
#define INIT_SPACE_HPP

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class init_space : public QWidget
{
private:

public:
    init_space(int width, QString label = "");
    ~init_space();
};

#endif //INIT_SPACE_HPP