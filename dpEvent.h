/* zyhdEvent.h - windows style's event routine. compatible windows and vxworks
 *
 * �����ṩ��EVENT���˹�����ģ���֧���Զ����飡
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
