#include <knob_file.h>
#include <vinacomp.h>

knob_file::knob_file(knob_props props, QString file_path)
    : knob(props)
    , save_file_dialog(props.knob_data.value("save_file_dialog").toBool())
    , allowed_file_types(props.knob_data.value("allowed_file_types").toArray())
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
    qt::set_icon(file_open, "folder_normal", 20);
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
    file_dialog *dialog = static_cast<vinacomp *>(_vinacomp)->get_file_dialog();

    dialog->set_file_mode();
    if (save_file_dialog)
        dialog->set_save_mode();
    else
        dialog->set_open_mode();

    QStringList filters;
    for (QJsonValue value : allowed_file_types)
        filters.push_back(value.toString());

    dialog->set_file_filter(filters);

    if (dialog->exec())
    {
        QString file = dialog->get_files().first();
        filename->setText(file);
        changed(file);
        update_value(file);
    }
}
