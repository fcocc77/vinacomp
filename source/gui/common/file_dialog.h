#ifndef FILE_DIALOG_H
#define FILE_DIALOG_H

#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTreeWidget>
#include <QSplitter>

#include <tools.h>
#include <combo_box.h>

class file_dialog : public QDialog
{
private:
    QVBoxLayout *layout;

    tools *tool_bar;
    QWidget *center_widget, *bottom_widget, *bottom_tools_widget;
    QLabel *preview_image;
    QTreeWidget *tree, *bookmark_tree;
    QList<QTreeWidgetItem*> items;

    QLineEdit *path_edit;
    combo_box *disk_path;
    combo_box *filter_box;
    bool preview_image_visible;
    QString current_path;
    QString current_filename;
    QPushButton *open_save_button;
    QLineEdit *create_folder_name;
    QSplitter *splitter;

    QStringList files;
    bool file_mode, save_mode;
    bool image_sequence;

    struct filter_struct
    {
        QString name;
        QStringList filters;
    };

    QMap<QString, filter_struct> filters;
    QStringList current_filters;

    struct bookmark
    {
        QString name;
        QString path;
        QTreeWidgetItem *item;
    };

    QMap<QString, bookmark> bookmarks;

    QString current_dir;
    void update();
    void go_to_parent();
    void enter_to_dir(QString dirname);
    void add_bookmark();
    void add_bookmark(QString dirname, QString dirpath);
    void remove_bookmark();
    void open_bookmark(QString bookmark);
    void switch_preview_image();
    void set_preview_image(QString image_basename);
    void open_or_save();
    void bookmark_backup();
    void filter(QString _filter);
    void update_filter_box();
    QStringList filter_by_sequence(QStringList files);
    void create_directory();
    void center();

public:
    file_dialog(QWidget *parent);
    ~file_dialog();

    int exec() override;
    inline QStringList get_files() const;
    void set_init_directory(QString directory);
    void set_name_filter(QString name_filter, QStringList filters);
    void set_names_filters(QList<filter_struct> filters);
    inline void set_file_mode();
    inline void set_dir_mode();
    inline void set_save_mode();
    inline void set_open_mode();
};

inline QStringList file_dialog::get_files() const
{
    return files;
}

inline void file_dialog::set_file_mode()
{
    file_mode = true;
}

inline void file_dialog::set_dir_mode()
{
    file_mode = false;
}

inline void file_dialog::set_save_mode()
{
    save_mode = true;
}

inline void file_dialog::set_open_mode()
{
    save_mode = false;
}

#endif // FILE_DIALOG_H
