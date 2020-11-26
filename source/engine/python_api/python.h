// los .h de Qt definen "slots" para otra cosa,
// si esta definido 'slots' , 'Python.h' da conflicto
#ifdef slots
#undef slots
#endif
#include "Python.h"
//