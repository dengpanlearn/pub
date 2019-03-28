/* SyncObj.h */
#if defined(_WIN32)
 #include "private/win/SyncObj.h"
#elif defined(__VXWORKS__)
 #include "private/vxw/SyncObj.h"
#elif defined(_LINUX)
#include "private/linux/SyncObj.h"
#else
 #error
#endif
