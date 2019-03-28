/* dpAtomic.h - ZYHD atomic routinues
 *
 */
#if defined(_WIN32)
 #include "private/win/dpAtomic.h"
#elif defined(__VXWORKS__)
 #include "private/vxw/dpAtomic.h"
#elif defined(_LINUX)
#include "private/linux/dpAtomic.h"
#else
 #error
#endif
