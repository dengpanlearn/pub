/*dp.h*/

#ifndef DP_PUBLIC_H
#define DP_PUBLIC_H

#ifdef __cplusplus
 #define EXTRN	extern "C"
#else
 #define EXTRN	extern
#endif

#if defined(__GNUC__)
 #undef  ATTRIB_PACKED
 #define ATTRIB_PACKED	__attribute__((packed))
#else
 #define ATTRIB_PACKED
#endif

#if defined(_WIN32)
 #include "private/win/dpWin.h"
#elif defined(__VXWORKS__)
 #include "private/vxw/zyhdvxw.h"
#elif defined(_LINUX)
#include "private/linux/dpLinux.h"
#else
 #error
#endif

/* 4 字节对齐的copy, zero, 赋值, 交换 */
#if defined(_WIN32)
 #define FASTCOPY(src, dst, nbytes) memcpy(dst, src, nbytes)
 #define FASTZERO(dst, nbytes)		memset(dst, 0, nbytes)
 #define FASTMOVE(src, dst, nbytes) memmove(dst, src, nbytes)
#else
 #define FASTCOPY(src, dst, nbytes) bcopyLongs((char*)src, (char*)dst, (nbytes)/sizeof(int))
 #define FASTZERO(dst, nbytes)		bfillLongs((char*)dst, nbytes/sizeof(int), 0)
 #define FASTMOVE(src, dst, nbytes) bcopyLongs((char*)src, (char*)dst, (nbytes)/sizeof(int))
#endif

/* nbytes 是常量时的优化 */
#if defined(_WIN32) || defined(_GCC_BUILTIN_)
 #define CONSTCOPY(src, dst, nbytes) 	memcpy(dst, src, nbytes)
 #define CONSTZERO(dst, nbytes)			memset(dst, 0, nbytes)
#else
 #define CONSTCOPY(src, dst, nbytes)	bcopyLongs((char*)src, (char*)dst, (nbytes)/sizeof(int))
 #define CONSTZERO(dst, nbytes)			bfillLongs((char*)dst, nbytes/sizeof(int), 0)
#endif

#define FASTEQU(left, right) \
		if (sizeof(left)==sizeof(int)*2) \
		{ \
			int* pa = (int*)&(left); \
			int* pb = (int*)&(right); \
			pa[0] = pb[0]; \
			pa[1] = pb[1]; \
		} \
		else if (sizeof(left)==sizeof(int)) \
		{ \
			int* pa = (int*)&(left); \
			int* pb = (int*)&(right); \
			pa[0] = pb[0]; \
		} \
		else if ((sizeof(left)%sizeof(int))==0) \
		{ \
			FASTCOPY(&right, &left, sizeof(left));\
		} \
		else \
		{ \
			ASSERT(FALSE);\
		}

#define FASTSWAP(a, b) \
		if (sizeof(a)==sizeof(int)*2) \
		{ \
			int* pa = (int*)&(a); \
			int* pb = (int*)&(b); \
			int t = pa[0]; \
			pa[0] = pb[0]; \
			pb[0] = t; \
			t = pa[1]; \
			pa[1] = pb[1]; \
			pb[1] = t; \
		} \
		else if (sizeof(a)==sizeof(int)) \
		{ \
			int* pa = (int*)&(a); \
			int* pb = (int*)&(b); \
			int t = pa[0]; \
			pa[0] = pb[0]; \
			pb[0] = t; \
		} \
		else \
		{ \
			ASSERT(FALSE);\
		}

#define STATUS_OK				0
#define STATUS_ERROR			-1

#ifdef DEBUG

#endif // DEBUG

#define	DP_UNUSED_EX(_x)
#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((int) &((TYPE *)0)->MEMBER)
#endif // !offsetof

#define member_to_object(pMember, type, memberName) \
    ((type *)((char *)(pMember) - offsetof (type, memberName)))
#endif /* ZYHD_PUBLIC_H */
