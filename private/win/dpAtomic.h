/* dpAtomic.h - DP atomic routinues for vxWorks
 *
 */
#ifndef __DP_ATOMIC_H__
#define __DP_ATOMIC_H__

typedef LONG	DP_ATOMIC;

/* 将原子变量v清零 */
#define DP_ATOMIC_ZERO(v)					_InterlockedExchange(&v, 0)

/* 将原子变量v清零并返回之前的值到result中 */
#define DP_ATOMIC_ZERO2(v, result)		result = _InterlockedExchange(&v, 0)

#define DP_ATOMIC_INC(v)					 _InterlockedIncrement(&v)
#define DP_ATOMIC_DEC(v)					_InterlockedDecrement(&v)

#define DP_ATOMIC_ADD(v, delta)			_InterlockedAdd(&v, delta)
#define DP_ATOMIC_SUB(v, delta)			_InterlockedAdd(&v, -delta)

#define DP_ATOMIC_GET(v)					_InterlockedOr(&v, 0)
#define DP_ATOMIC_SET(v, newVal)			_InterlockedExchange(&v, newVal)
#define	DP_ATOMIC_CAS(v, oldVal, newVal)	(_InterlockedCompareExchange(&v, newVal, oldVal)==oldVal)

/* 将原子变量v设置为newVal并返回之前的值到result中 */
#define DP_ATOMIC_SET2(v, newVal, result)	result = _InterlockedExchange(&v, newVal)

#define DP_ATOMIC_BITS_SET(v, bits)		_InterlockedOr(&v, bits)
#define DP_ATOMIC_BITS_CLR(v, bits)		_InterlockedAnd(&v, (~(bits)))

#endif /* __DP_ATOMIC_H__ */
