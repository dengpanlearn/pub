/*dpWin.h
**/

#ifndef DP_PUBLIC_H
 #error "Can NOT include this file, include dp.h"
#endif

#define CPU_FAMILY	I80X86

/* �ֽ��� */
#undef  _BIG_ENDIAN
#define _BIG_ENDIAN		1234

#undef  _LITTLE_ENDIAN
#define _LITTLE_ENDIAN	4321

#define _BYTE_ORDER		_LITTLE_ENDIAN

/*
Windowsƽ̨����ͷ�ļ�����
*/
#include <winsock2.h>
#include <windows.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <new.h>
#include <malloc.h>
#include <intrin.h>
#include <tchar.h>
#pragma comment(lib, "Ws2_32")

typedef u_long	in_addr_t;

#undef  EWOULDBLOCK
#define EWOULDBLOCK		WSAEWOULDBLOCK

#define SIZE_INT			32	/* ������32λ */
#define SIZE_LONG			32	/* ��������32λ */
#define SIZE_PTR			32	/* ָ����32λ */

/* �������Ͷ���
���飺����ʹ��UINT,INT����ʹ�ô���ȵ���������
*/
typedef unsigned int		UINT;		/* �޷������� */
typedef unsigned __int64	UINT64;		/* 64λ�޷������� */
typedef unsigned int		UINT32;		/* 32λ�޷������� */
typedef unsigned short		UINT16;		/* 16λ�޷������� */
typedef unsigned char		UINT8;		/* 8λ�޷������� */
#ifndef _BASETSD_H_
 typedef unsigned long		UINT_PTR;	/* ��ָ��ȿ���޷������� */
#endif

typedef signed int			INT;	/* �з������� */
typedef signed __int64		INT64;	/* 64λ�з������� */
typedef signed int			INT32;	/* 32λ�з������� */
typedef signed short		INT16;	/* 16λ�з������� */
typedef signed char			INT8;	/* 8λ�з������� */

typedef int (*FUNCPTR)(...);
typedef int				STATUS;
#define LOCAL			static

#define I64_FMT			"I64"	/* 64λ������ʽ����� */

#define MAX_INT			((INT)0x7fffffff)
#define MIN_INT			((INT)0x80000000)
#define MAX_UINT		((UINT)0xffffffffU)

#define MAX_INT64		((INT64)0x7fffffffffffffff)
#define MIN_INT64		((INT64)0x8000000000000000)
#define MAX_UINT64		((UINT64)0xffffffffffffffffU)

#define MAX_INT32		((INT32)0x7fffffff)
#define MIN_INT32		((INT32)0x80000000)
#define MAX_UINT32		((UINT32)0xffffffffU)

#define MAX_INT16		((INT16)0x7fff)
#define MIN_INT16		((INT16)0x8000)
#define MAX_UINT16		((UINT16)0xffffU)

#define MAX_INT8		((INT8)0x7f)
#define MIN_INT8		((INT8)0x80)
#define MAX_UINT8		((UINT8)0xffU)

/* �ֽ��򽻻��ĺ� */
#define LLSB(x)	 ((x) & 0xff)
#define LNLSB(x) (((x) >> 8) & 0xff)
#define LNMSB(x) (((x) >> 16) & 0xff)
#define LMSB(x)	 (((x) >> 24) & 0xff)

/* �����ֽ��� -> С���ֽ���, ��С���ֽ��� -> �����ֽ��� */
#define LITTLE_SWAP(x)		(x)
#define LITTLE_SWAP_I32(x)	(x)
#define LITTLE_SWAP_I16(x)	(x)

/* �����ֽ��� -> ����ֽ���, �����ֽ��� -> �����ֽ��� */
#define BIG_SWAP(x)		((LLSB(x) << 24) | (LNLSB(x) << 16) | (LNMSB(x) << 8) | (LMSB(x)))
#define BIG_SWAP_I32(x)	BIG_SWAP(x)
#define BIG_SWAP_I16(x)	((LLSB(x)<<8) | LNLSB(x))

/* ����Ԫ����Ŀ����� */
#ifndef countof
 #define countof(__x)	(sizeof(__x)/sizeof(__x[0]))
#endif /* countof */

/* ȡ�ṹ����ĳ��Ա������ƫ�� */
#ifndef _INC_STDDEF
 #ifndef offsetof
  #define offsetof(structure, member) ((UINT_PTR)&((structure*)0)->member)
 #endif
#endif

/* ȡ�ṹ����ĳ��Ա�����Ĵ�С */
#define MEMBER_SIZE(structure, member) 	(sizeof(((structure*)0)->member))
#define member_size(structure, member) 	(sizeof(((structure*)0)->member))

/* ��ѧ���� */
#define PI			3.1415926535897932384626433832795
#define SQRT2		1.4142135623730950488016887242097
#define SQRT3		1.7320508075688772935274463415059
#define F577		(100.0/SQRT3)	/* 57.735 */

#define bcopy(src,dst,n)	memcpy(dst, src, n)
#define bzero(src,n)		ZeroMemory(src,n)
#define errnoSet(x)			SetLastError(x)
#define errnoGet			GetLastError
#define stricmp				_stricmp
#define strnicmp			_strnicmp

#define inet_ntoa_b(addr, text)	strcpy(text, inet_ntoa(addr))
#define INET_ADDR_LEN		32

#define EOS		'\0'

#ifndef ENOTSUP
 #define ENOTSUP 35
#endif

#define WAIT_FOREVER	(UINT)(-1)
#define OK				0
typedef int				STATUS;

#include "dllLib.h"

#define msDelay(ms)		Sleep(ms)
#define msTickCountGet	GetTickCount

#define ROUND_UP(x, align)		(((long)(x) + ((long)align - 1)) & (~((long)align - 1)))
#define ROUND_DOWN(x, align)	((long)(x) & ~((long)align - 1))

/* Debug��� */
#ifdef _MFC_VER
 #define dbgTrace		AfxTrace
 #define logMsg			AfxTrace
 #ifdef _DEBUG
  #define malloc(x)		AfxAllocMemoryDebug(x, FALSE, THIS_FILE, __LINE__)
  #define calloc(x,y)	memset(AfxAllocMemoryDebug((x)*(y), FALSE, THIS_FILE, __LINE__), 0, (x)*(y))
  #define free(x)		AfxFreeMemoryDebug(x, FALSE)
 #endif
#else /* !_MFC_VER */
 #ifdef __cplusplus
  extern "C" {
 #endif
 void zyhdTrace(const TCHAR* fmt, ...);

 inline int WcharToChar(LPCTSTR pwchar, char* pChar)
 {
	 int iLength = WideCharToMultiByte(CP_ACP, 0, pwchar, wcslen(pwchar), NULL, 0, NULL, NULL);
	 if (iLength < 0)
		 return iLength;

	 return WideCharToMultiByte(CP_ACP, 0, pwchar, wcslen(pwchar), pChar, iLength, NULL, NULL);
 }


 #ifdef __cplusplus
  }
 #endif
 #define dbgTrace		zyhdTrace
 #define logMsg			zyhdTrace
 #define UNUSED_ALWAYS(x)	(void)(x)
 #define UNUSED_EX(x) 		(void)(x)
 #ifdef _DEBUG
  #include <assert.h>
  #define ASSERT(x)		assert(x)
  #define TRACE			zyhdTrace
 #else
  #define ASSERT(x)
  #define TRACE			1 ? 1 : _tprintf
 #endif
#endif /* _MFC_VER */

#ifdef _CONSOLE
 #undef  logMsg
 #define logMsg(fmt,a,b,c,d,e,f)	printf(fmt,a,b,c,d,e,f)
#endif

/* ����WinGDI.h �� ERROR ����Ϊ0�� ��vxWorks/Linux����ΪERRORΪ-1��𼫴�
 * ���ԣ���ƽ̨����(Windows��)��ֹʹ��ERROR
 */
#undef ERROR
