#include <file_dialog.h>

file_dialog::file_dialog(QWidget *parent)
    : QWidget(parent)
{
    layout = new QVBoxLayout(this);

    // Tool Bar
    tool_bar = new tools();
    action *go_back_action = new action("Go Back");
    action *create_folder = new action("Create Directorio");

    tool_bar->add_stretch();
    tool_bar->add_action(go_back_action);
    tool_bar->add_action(create_folder);
    //

    // Center
    center_widget = new QWidget;
    QHBoxLayout *center_layout = new QHBoxLayout(center_widget);

    tag_widget = new QWidget;

    directory_widget = new QWidget;
    QVBoxLayout *directory_layout = new QVBoxLayout(directory_widget);
    tree = new QTreeWidget;
    QLineEdit *path_edit = new QLineEdit;
    directory_layout->addWidget(tree);
    directory_layout->addWidget(path_edit);

    preview_widget = new QWidget;

    center_layout->addWidget(tag_widget);
    center_layout->addWidget(directory_widget);
    center_layout->addWidget(preview_widget);
    //

    // Bottom Widget
    bottom_widget = new QWidget;
    QHBoxLayout *bottom_layout = new QHBoxLayout(bottom_widget);
    QLabel *filter_label = new QLabel("Filter:");
    QLineEdit *filter_line = new QLineEdit;
    QPushButton *open_save_button = new QPushButton("Open");
    QPushButton *cancel_button = new QPushButton("Cancel");
    bottom_layout->addWidget(filter_label);
    bottom_layout->addWidget(filter_line);
    bottom_layout->addWidget(open_save_button);
    bottom_layout->addWidget(cancel_button);

    //

    layout->addWidget(tool_bar);
    layout->addWidget(center_widget);
    layout->addWidget(bottom_widget);
}

file_dialog::~file_dialog() {}
