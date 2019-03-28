/* SyncObj.cpp */
#include <stdlib.h>
#include <stdio.h>
#include "dp.h"

#if defined(_WIN32)
 #include "private/win/SyncObj.hpp"
#elif defined(__VXWORKS__)
 #include "private/vxw/SyncObj.hpp"
#elif defined(_LINUX)
#include "private/linux/SyncObj.hpp"
#else
 #error
#endif
