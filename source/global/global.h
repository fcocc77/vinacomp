#ifndef GLOBAL_H
#define GLOBAL_H

#define USER_DIR QString::fromStdString(getenv("HOME"))

#define VINACOMP_CONF_PATH USER_DIR + "/.vinacomp"

#define PY_PLUGINS_PATH QString("plugins/py_plugins")

#endif // GLOBAL_H
