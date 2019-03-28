/*dpAtomic.h
 *
 * linux系统在应用层面没有提供原子操作api接口,因此,在Linux系统应用程序下，使用gcc提供的原子操作提供api接口。
 *
 * gcc4.1.2版本及以上才支持原子操作
 * */

#ifndef __DP_ATOMIC_H__
#define __DP_ATOMIC_H__

#include <stdio.h>
#include <stdlib.h>
typedef long	DP_ATOMIC;


/* 将原子变量v清零 */
#define DP_ATOMIC_ZERO(v)					__sync_lock_release(&v)

/* 将原子变量v清零并返回之前的值到result中 */
#define DP_ATOMIC_ZERO2(v, result)			result = __sync_fetch_and_and(&v, 0)

/*原子变量自增、自减操作*/
#define DP_ATOMIC_INC(v)					__sync_add_and_fetch(&v, 1)
#define DP_ATOMIC_DEC(v)					__sync_sub_and_fetch(&v, 1)

/*原子变量加、减操作*/
#define DP_ATOMIC_ADD(v, delta)				__sync_add_and_fetch(&v, delta)
#define DP_ATOMIC_SUB(v, delta)				__sync_sub_and_fetch(&v, delta)

/*原子变量获取、设置操作*/
#define DP_ATOMIC_GET(v)					__sync_add_and_fetch(&v, 0)
#define DP_ATOMIC_SET(v, newVal)			__sync_lock_test_and_set(&v, newVal)
#define	DP_ATOMIC_CAS(v, oldVal, newVal)	__sync_bool_compare_and_swap(&v, oldVal, newVal)

/* 将原子变量v设置为newVal并返回之前的值到result中 */
#define DP_ATOMIC_SET2(v, newVal, result)	result = __sync_lock_test_and_set(&v, newVal)

#define DP_ATOMIC_BITS_SET(v, bits)			__sync_or_and_fetch(&v, bits)
#define DP_ATOMIC_BITS_CLR(v, bits)			__sync_and_and_fetch(&v, ~(bits))

#endif	/*__DP_ATOMIC_H__*/
