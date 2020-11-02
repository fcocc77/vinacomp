#ifndef MAKER_HPP
#define MAKER_HPP

#include <QJsonObject>

#include <util.hpp>
#include <os.hpp>

class maker
{
private:
    QJsonObject effects;

public:
    maker(/* args */);
    ~maker();

    void create_fx(QString type);
    QJsonObject get_effect(QString type);
    QJsonObject get_effects();
};

#endif //MAKER_HPP