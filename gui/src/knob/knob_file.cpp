#include <knob_file.hpp>

knob_file::knob_file(/* args */)
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    QLabel *label = new QLabel("Filename");
    layout->addWidget(label);

    QLineEdit *filename = new QLineEdit("Filename");
    layout->addWidget(filename);

    QPushButton *file_open = new QPushButton();
    connect(file_open, &QPushButton::clicked, this, &knob_file::open_file);
    qt::set_icon(file_open, "folder_a", 20);
    layout->addWidget(file_open);
}

knob_file::~knob_file()
{
}

void knob_file::open_file()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);

    // dialog.setNameFilter();

    if (dialog.exec())
    {
    }
}