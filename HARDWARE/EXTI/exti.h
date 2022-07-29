#ifndef _EXTI_H
#define _EXTI_H
#include "sys.h"

/**
 * @brief 
 * ��ʼ��0��
 * ����д����1������¼ʱ�����
 * �������ظ�д�룬��д�루������
 * ��ȡ���ݺ�ȫ������
 */
struct exti_data
{
    u16 data_len; //���ݳ���
    u16 data_writed;    //��д�������λ��1
    u32 timestap[10];
} ;
extern struct exti_data ExtiData;


void EXTI_Init(void);
#endif
