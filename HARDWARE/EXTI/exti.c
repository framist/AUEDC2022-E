#include "exti.h"
#include "delay.h"
#include "led.h"
#include "key.h"


struct exti_data ExtiData = {0};

//�ⲿ�жϳ�ʼ��
void EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOC_CLK_ENABLE();               //����GPIOCʱ��
    
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;      //�����ش���
    GPIO_Initure.Pull=GPIO_PULLDOWN;            //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;
    GPIO_Initure.Pin=GPIO_PIN_0;                //PC0
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
    
    GPIO_Initure.Pin=GPIO_PIN_1;                //PC1
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
    
    GPIO_Initure.Pin=GPIO_PIN_2;                //PC2
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
    
    // GPIO_Initure.Pin=GPIO_PIN_3;                //PC3
    // HAL_GPIO_Init(GPIOC,&GPIO_Initure);
    
    // GPIO_Initure.Pin=GPIO_PIN_4;                //PC4
    // HAL_GPIO_Init(GPIOC,&GPIO_Initure);
    
    // GPIO_Initure.Pin=GPIO_PIN_5;                //PC5
    // HAL_GPIO_Init(GPIOC,&GPIO_Initure);

    //�ж���0-PC0
    HAL_NVIC_SetPriority(EXTI0_IRQn,2,0);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ0
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);             //ʹ���ж���0

    //�ж���1-PC1
    HAL_NVIC_SetPriority(EXTI1_IRQn,2,1);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ1
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);             //ʹ���ж���0
    
    //�ж���2-PC2
    HAL_NVIC_SetPriority(EXTI2_IRQn,2,2);   	//��ռ���ȼ�Ϊ2�������ȼ�Ϊ2
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);         	//ʹ���ж���4

    ExtiData.data_len = 3;
}


//�жϷ�����
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);		//�����жϴ����ú���
}

void EXTI1_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);		//�����жϴ����ú���
}

void EXTI2_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);		//�����жϴ����ú���
}

// void EXTI3_IRQHandler(void)
// {
//     HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);		//�����жϴ����ú���
// }

// void EXTI4_IRQHandler(void)
// {
//     HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);		//�����жϴ����ú���
// }

// void EXTI5_IRQHandler(void)
// {
//     HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);		//�����жϴ����ú���
// }


//�жϷ����������Ҫ��������
//��HAL�������е��ⲿ�жϷ�����������ô˺���
//GPIO_Pin:�ж����ź�
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    uint32_t nowTime = SysTick->VAL;
    switch (GPIO_Pin) {
    case GPIO_PIN_0:
        if ((ExtiData.data_writed & GPIO_Pin) == 0) {
            ExtiData.data_writed |= GPIO_Pin;
            ExtiData.timestap[0] = nowTime;
            // HAL_NVIC_DisableIRQ(EXTI0_IRQn);
        }
        break;
    case GPIO_PIN_1:
        if ((ExtiData.data_writed & GPIO_Pin) == 0) {
            ExtiData.data_writed |= GPIO_Pin;
            ExtiData.timestap[1] = nowTime;
            // HAL_NVIC_DisableIRQ(EXTI1_IRQn);
        }
        break;
    case GPIO_PIN_2:
        if ((ExtiData.data_writed & GPIO_Pin) == 0) {
            ExtiData.data_writed |= GPIO_Pin;
            ExtiData.timestap[2] = nowTime;
            // HAL_NVIC_DisableIRQ(EXTI2_IRQn);
        }
        break;
    default:
        break;
    }
    return;

}
