/*dpThread.cpp
**/

#include "dp.h"
#include "dpThread.h"

#if defined(_WIN32)
 /* nothing */
 /* #include "private/win/zyhdThread.hpp" */
#elif defined(__VXWORKS__)
 #include "private/vxw/zyhdThread.hpp"
#elif defined(_LINUX)
#include "private/linux/dpThread.hpp"
#else
 #error
#endif
