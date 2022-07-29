/**
 * @file timer.c
 * @author framist (framist@163.com)
 * @brief ͨ�ö�ʱ��TIM3 -> OS_TimeMS ��
 *        ͨ�ö�ʱ��TIM4 -> ����������
 *        ͨ�ö�ʱ��TIM5 -> ʱ��
 * @version 0.1
 * @date 2021-08-31
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "timer.h"
#include "led.h"
#include "GUI.h"
extern __IO int32_t OS_TimeMS; //GUI ��
extern __IO uint32_t nowTime;
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ���ж���������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/4/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//��ֲ������STemWin - framist
////////////////////////////////////////////////////////////////////////////////// 	


//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//999,83
//Ft = 84Mhz
//1KHZ ��ʱ��3����Ϊ1ms
TIM_HandleTypeDef 	TIM3_Handler;      	//��ʱ����� 
void TIM3_Init(u16 arr,u16 psc)
{  
    TIM3_Handler.Instance=TIM3;                          //ͨ�ö�ʱ��3
    TIM3_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM3_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM3_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM3_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE   
    //HAL_TIM_Base_Stop_IT(&TIM3_Handler);
}

TIM_HandleTypeDef 	TIM4_Handler;      	//��ʱ��4��� 
TIM_HandleTypeDef TIM14_Handler;      	//��ʱ����� 
TIM_OC_InitTypeDef TIM14_CH1Handler;	//��ʱ��14ͨ��1���
// 
void TIM4_Init(u16 arr,u16 psc)
{  
    TIM4_Handler.Instance=TIM4;                          //ͨ�ö�ʱ��4
    TIM4_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM4_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����=0
    HAL_TIM_Base_Init(&TIM4_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM4_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��4�����жϣ�TIM_IT_UPDATE
}

TIM_HandleTypeDef 	TIM5_Handler;      	//��ʱ��5��� 
// 
void TIM5_Init(u16 arr,u16 psc)
{  
    TIM5_Handler.Instance=TIM5;                          //ͨ�ö�ʱ��4
    TIM5_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM5_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM5_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM5_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����=0
    HAL_TIM_Base_Init(&TIM5_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM5_Handler); //ʹ��
    // HAL_TIM_Base_Stop_IT(&TIM5_Handler); //�ر�
}


//TIM14 PWM���ֳ�ʼ�� 
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
void TIM14_PWM_Init(u16 arr,u16 psc)
{  
    TIM14_Handler.Instance=TIM14;          	//��ʱ��14
    TIM14_Handler.Init.Prescaler=psc;       //��ʱ����Ƶ
    TIM14_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//���ϼ���ģʽ
    TIM14_Handler.Init.Period=arr;          //�Զ���װ��ֵ
    TIM14_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM14_Handler);       //��ʼ��PWM
    
    TIM14_CH1Handler.OCMode=TIM_OCMODE_PWM1; //ģʽѡ��PWM1
    TIM14_CH1Handler.Pulse=arr/2;            //���ñȽ�ֵ,��ֵ����ȷ��ռ�ձȣ�Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50%
    TIM14_CH1Handler.OCPolarity=TIM_OCPOLARITY_LOW; //����Ƚϼ���Ϊ�� 
    HAL_TIM_PWM_ConfigChannel(&TIM14_Handler,&TIM14_CH1Handler,TIM_CHANNEL_1);//����TIM14ͨ��1
	
    HAL_TIM_PWM_Start(&TIM14_Handler,TIM_CHANNEL_1);//����PWMͨ��1
}


//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������
// StemWin ��Ҫ
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();            //ʹ��TIM4ʱ��
		HAL_NVIC_SetPriority(TIM4_IRQn,2,3);    //�����ж����ȼ�����ռ���ȼ�2�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM4_IRQn);          //����TIM4�ж�   
	}
    if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();            //ʹ��TIM3ʱ��
		HAL_NVIC_SetPriority(TIM3_IRQn,1,3);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM3_IRQn);          //����ITM3�ж�   
	}
    if(htim->Instance==TIM5)
	{
		__HAL_RCC_TIM5_CLK_ENABLE();            //ʹ��TIM5ʱ��
		HAL_NVIC_SetPriority(TIM5_IRQn,1,2);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�2
		HAL_NVIC_EnableIRQ(TIM5_IRQn);          //����ITM5�ж�   
	}
    if(htim->Instance==TIM6) {
        /* TIM6 clock enable */
        __HAL_RCC_TIM6_CLK_ENABLE();
    }
}

//��ʱ���ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_TIM_PWM_Init()����
//htim:��ʱ�����
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_TIM14_CLK_ENABLE();			//ʹ�ܶ�ʱ��14
	__HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOAʱ��
	
	GPIO_Initure.Pin=GPIO_PIN_7;           	//PA7
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//�����������
	GPIO_Initure.Pull=GPIO_PULLUP;          //����
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
	GPIO_Initure.Alternate= GPIO_AF9_TIM14;	//PF7����ΪTIM14_CH1
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}

//����TIMͨ��4��ռ�ձ�
//compare:�Ƚ�ֵ
void TIM_SetTIM14Compare1(u32 compare)
{
	TIM14->CCR1=compare; 
}


//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void){
    HAL_TIM_IRQHandler(&TIM3_Handler);
}

//��ʱ��4�жϷ�����
void TIM4_IRQHandler(void){
    HAL_TIM_IRQHandler(&TIM4_Handler);
}

//��ʱ��5�жϷ�����
void TIM5_IRQHandler(void){
    HAL_TIM_IRQHandler(&TIM5_Handler);
}

//�ص���������ʱ���жϷ���������

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    
    if(htim==(&TIM4_Handler)){
        GUI_TOUCH_Exec();    //��ʱ���������¼�
    }
    if(htim==(&TIM3_Handler)){
        OS_TimeMS++;         //ϵͳ����
    }
    if(htim==(&TIM5_Handler)){  
        nowTime++;
    }

}

