/*dpLinux.h
 *
 * */

#ifndef DP_PUBLIC_H
 #error "Can NOT include this file, include dp.h"
#endif

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

/*数据类型定义*/
typedef __u_char	UINT8;
typedef	__u_short	UINT16;
typedef __u_int		UINT;
typedef	__uint64_t	UINT64;
typedef	__int8_t	INT8;
typedef	__int16_t	INT16;
typedef	__int32_t	INT;
typedef	__int64_t	INT64;

#ifndef _BASETSD_H_
 typedef unsigned long		UINT_PTR;	/* 与指针等宽的无符号整数 */
#endif

typedef int (*FUNCPTR)(...);
typedef int			STATUS;
#define LOCAL		static

typedef	int			BOOL;
#define	TRUE		1
#define	FALSE		0

#define	OK			0
#define	ERROR		(-1)

typedef char			TCHAR;
typedef const char*		LPCTSTR;
typedef char*			LPTSTR;
#define EOS				'\0'
#define _T(_x)		(_x)

/*数据类型范围*/

#define MAX_INT			((INT)0x7fffffff)
#define MIN_INT			((INT)0x80000000)
#define MAX_UINT		((UINT)0xffffffffU)

#define MAX_INT64		((INT64)0x7fffffffffffffffull)
#define MIN_INT64		((INT64)0x8000000000000000ull)
#define MAX_UINT64		((UINT64)0xffffffffffffffffull)

#define MAX_INT32		((INT32)0x7fffffff)
#define MIN_INT32		((INT32)0x80000000)
#define MAX_UINT32		((UINT32)0xffffffffU)

#define MAX_INT16		((INT16)0x7fff)
#define MIN_INT16		((INT16)0x8000)
#define MAX_UINT16		((UINT16)0xffffU)

#define MAX_INT8		((INT8)0x7f)
#define MIN_INT8		((INT8)0x80)
#define MAX_UINT8		((UINT8)0xffU)

#define	INFINITE		((UINT)-1)


/* 字节序交换的宏 */
#define LLSB(x)	 		((x) & 0xff)
#define LNLSB(x) 		(((x) >> 8) & 0xff)
#define LNMSB(x) 		(((x) >> 16) & 0xff)
#define LMSB(x)	 		(((x) >> 24) & 0xff)


/* 本地字节序 -> 小端字节序, 或小端字节序 -> 本地字节序 */
#define LITTLE_SWAP(x)		htole32(x)
#define LITTLE_SWAP_I64(x)	htole64(x)
#define LITTLE_SWAP_I32(x)	htole32(x))
#define LITTLE_SWAP_I16(x)	htole16(x)

/* 本地字节序 -> 大端字节序, 或大端字节序 -> 本地字节序 */
#define BIG_SWAP(x)			htobe32(x)
#define BIG_SWAP_I64(x)		htobe64(x)
#define BIG_SWAP_I32(x)		htobe32(x)
#define BIG_SWAP_I16(x)		htobe16(x)

/* 数组元素数目运算符 */
#ifndef countof
 #define countof(__x)	(sizeof(__x)/sizeof(__x[0]))
#endif /* countof */

/* 取结构体中某成员变量的偏移 */
#ifndef _INC_STDDEF
 #ifndef offsetof
  #define offsetof(structure, member) ((UINT_PTR)&((structure*)0)->member)
 #endif
#endif

#ifdef	_DEBUG
#include <assert.h>
#define ASSERT(x)		assert(x)
#else
#define ASSERT(x)
#endif
#define ROUND_UP(x, align)		(((long)(x) + ((long)align - 1)) & (~((long)align - 1)))
#define ROUND_DOWN(x, align)	((long)(x) & ~((long)align - 1))

#ifdef __cplusplus
 extern "C" {
#endif

inline int stricmp(const char *s, const char *t)
{
	return strcasecmp(s, t);
}

inline int strnicmp(const char *s, const char *t, size_t len)
{
	return strncasecmp(s, t, len);
}
inline void charTowchar(const char *chr, LPTSTR pWchar, int size)
{
}

inline int sprintf_s(char* pBuf, size_t sizeOfBuffer, const char* format, ...)
{
	va_list list;
	int i;
	
	va_start(list, format);
	i = _vsnprintf(pBuf, sizeOfBuffer, format, list);
	va_end(list);
	return i;
}

inline int swprintf_s(TCHAR* pBuf, size_t sizeOfBuffer, const TCHAR* format, ...)
{
	va_list list;
	int i;
	
	va_start(list, format);
	i = _vsnprintf(pBuf, sizeOfBuffer, format, list);
	va_end(list);
	return i;
}

#ifdef __cplusplus
 }
#endif
