/*dpThread.h
**/

#if defined(_WIN32)
 #include "private/win/dpThread.h"
#elif defined(__VXWORKS__)
 #include "private/vxw/dpThread.h"
#elif defined(_LINUX)
#include "private/linux/dpThread.h"
#else
 #error
#endif
