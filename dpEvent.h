/* zyhdEvent.h - windows style's event routine. compatible windows and vxworks
 *
 * 本库提供的EVENT是人工复归的，不支持自动复归！
 *
 */

#if defined(_WIN32)
 #include "private/win/dpEvent.h"
#elif defined(__VXWORKS__)
 #include "private/vxw/dpEvent.h"
#elif defined(_LINUX)
 #include "private/linux/dpEvent.h"
#else
 #error
#endif
