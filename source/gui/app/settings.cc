#include <settings.h>

settings::settings(/* args */)
{
    this->hide();

    layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    QTreeWidget *tree = new QTreeWidget();
    layout->addWidget(tree);
}

settings::~settings() {}
