/*dpAtomic.h
 *
 * linuxϵͳ��Ӧ�ò���û���ṩԭ�Ӳ���api�ӿ�,���,��LinuxϵͳӦ�ó����£�ʹ��gcc�ṩ��ԭ�Ӳ����ṩapi�ӿڡ�
 *
 * gcc4.1.2�汾�����ϲ�֧��ԭ�Ӳ���
 * */

#ifndef __DP_ATOMIC_H__
#define __DP_ATOMIC_H__

#include <stdio.h>
#include <stdlib.h>
typedef long	DP_ATOMIC;


/* ��ԭ�ӱ���v���� */
#define DP_ATOMIC_ZERO(v)					__sync_lock_release(&v)

/* ��ԭ�ӱ���v���㲢����֮ǰ��ֵ��result�� */
#define DP_ATOMIC_ZERO2(v, result)			result = __sync_fetch_and_and(&v, 0)

/*ԭ�ӱ����������Լ�����*/
#define DP_ATOMIC_INC(v)					__sync_add_and_fetch(&v, 1)
#define DP_ATOMIC_DEC(v)					__sync_sub_and_fetch(&v, 1)

/*ԭ�ӱ����ӡ�������*/
#define DP_ATOMIC_ADD(v, delta)				__sync_add_and_fetch(&v, delta)
#define DP_ATOMIC_SUB(v, delta)				__sync_sub_and_fetch(&v, delta)

/*ԭ�ӱ�����ȡ�����ò���*/
#define DP_ATOMIC_GET(v)					__sync_add_and_fetch(&v, 0)
#define DP_ATOMIC_SET(v, newVal)			__sync_lock_test_and_set(&v, newVal)
#define	DP_ATOMIC_CAS(v, oldVal, newVal)	__sync_bool_compare_and_swap(&v, oldVal, newVal)

/* ��ԭ�ӱ���v����ΪnewVal������֮ǰ��ֵ��result�� */
#define DP_ATOMIC_SET2(v, newVal, result)	result = __sync_lock_test_and_set(&v, newVal)

#define DP_ATOMIC_BITS_SET(v, bits)			__sync_or_and_fetch(&v, bits)
#define DP_ATOMIC_BITS_CLR(v, bits)			__sync_and_and_fetch(&v, ~(bits))

#endif	/*__DP_ATOMIC_H__*/
