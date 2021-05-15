#include <os.h>
#include <path_utils.h>

namespace path_util
{

QString get_ext(QString _string)
{
    return _string.split(".").last();
}

QString remove_ext(QString _string)
{
    QString basename = os::basename(_string);
    QString dirname = os::dirname(_string);

    return dirname + "/" + basename.replace("." + get_ext(basename), "");
}

QString get_padding(QString _string)
{
    QString basename = os::basename(_string);
    QString ext = get_ext(basename);
    basename = basename.replace("." + ext, "");

    std::reverse(basename.begin(), basename.end());

    QString padding;
    for (QChar _char : basename)
    {
        if (_char.isDigit())
            padding += _char;
        else
            break;
    }

    std::reverse(padding.begin(), padding.end());

    return padding;
}

QString remove_padding(QString _string)
{
    QString basename = os::basename(_string);
    QString dirname = os::dirname(_string);

    QString basename_no_padding = basename.replace(get_padding(basename), "");

    return dirname + "/" + basename_no_padding;
}

}; // namespace path_util
