#ifndef KNOB_FILE_HPP
#define KNOB_FILE_HPP

#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

#include <knob.h>
#include <qt.h>

class knob_file : public knob
{
    Q_OBJECT
private:
    QPushButton *file_open;
    QLineEdit *filename;

    QString default_value;

    bool save_file_dialog;
    QJsonArray allowed_file_types;

    void open_file();
    void restore_default() override;
    void restore_param() override;

public:
    knob_file(knob_props props, QString default_value = "");
    ~knob_file();

    inline QString get_value() const;
signals:
    void changed(QString file_path);
};

inline QString knob_file::get_value() const
{
    return filename->text();
}

#endif // KNOB_FILE_HPP
