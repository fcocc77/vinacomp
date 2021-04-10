#include <knob_file.h>

knob_file::knob_file(QString file_path)
{
    this->setObjectName("knob_file");
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);

    layout->addWidget(init_space);

    filename = new QLineEdit(file_path);
    connect(filename, &QLineEdit::editingFinished, this, [=]() {
        QString value = filename->text();
        changed(value);
        update_value(value);
    });
    layout->addWidget(filename);

    file_open = new QPushButton();
    connect(file_open, &QPushButton::clicked, this, &knob_file::open_file);
    qt::set_icon(file_open, "folder_a", 20);
    layout->addWidget(file_open);
}

knob_file::~knob_file()
{
    delete file_open;
}

void knob_file::restore_param()
{
    QJsonValue value = get_param_value();
    filename->setText(value.toString());
}

void knob_file::open_file()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);

    // dialog.setNameFilter();

    if (dialog.exec())
    {
        QString file = dialog.selectedFiles().value(0);
        filename->setText(file);
        changed(file);
        update_value(file);
    }
}
