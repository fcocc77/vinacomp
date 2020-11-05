#ifndef COMBO_BOX_HPP
#define COMBO_BOX_HPP

#include <QComboBox>

class combo_box : public QComboBox
{
private:
    /* data */
public:
    combo_box(/* args */);
    ~combo_box();

    void add_items(QStringList list);
};

#endif // COMBO_BOX_HPP