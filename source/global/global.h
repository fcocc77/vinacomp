#ifndef GLOBAL_H
#define GLOBAL_H

#define USER_DIR QString::fromStdString(getenv("HOME"))

#define VINACOMP_CONF_PATH USER_DIR + "/.vinacomp"

#endif // GLOBAL_H
