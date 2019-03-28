/* zyhdEvent.cpp - zyhdEvent */
#include <stdlib.h>
#include <stdio.h>
#include "dp.h"

#if defined(_WIN32)
 #include "private/win/dpEvent.hpp"
#elif defined(__VXWORKS__)
 #include "private/vxw/zyhdEvent.hpp"
#elif defined(_LINUX)
 #include "private/linux/dpEvent.hpp"
#else
 #error
#endif
