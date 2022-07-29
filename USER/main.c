/************************************************
fork from F4_HAL_emwin https://github.com/framist/STemWinForHAL
ʵ��ƽ̨ ALIENTEK STM32F407ZGT6��Сϵͳ��
AUEDC 2022

�漶�ӿڣ�

printf ������Ϊ�������

TIM14 PA7 ����PWM���
PC0~PC5 ������������
PF8 ��Ϊ����ʿ������

���һʱˬ���ع����᳡��
************************************************/

#include "main.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "ILI93xx.h"
#include "key.h"
#include "sram.h"
#include "malloc.h"
#include "touch.h"
#include "timer.h"
#include "arm_math.h"  
#include "adc.h"
#include "exti.h"
//GUI֧��
#include "GUI.h"
#include "FramewinDLG.h"



// ���� DS3120 �Ƕ�
// 1000~2500ns -> 0~270 deg
void PWM_SetDegree(double deg)
{
    LASER(GPIO_PIN_RESET);   //�رռ����
    deg += 90;
    deg = deg * 90 / 106; // ����
    u32 compare = 5000 - (u32)(deg/180.0 * 1500 + 1000);
	TIM_SetTIM14Compare1(compare);
    // mainLogPrintf("\nset deg: %f deg, %u cmp",deg,compare);
    LASER(GPIO_PIN_SET);   
}

// �����㺯��
#define NOW_TIME_TICK (1/168000000.0)
#include "math.h"
void main_solve(struct exti_data *ed) {
    //mainLogPrintf("\nmain_solve:");
    if (ed->data_writed == 0x0001U+0x0002U+0x0004U) {
        double t1 = ed->timestap[0]*NOW_TIME_TICK;
        double t2 = ed->timestap[1]*NOW_TIME_TICK;
        double t3 = ed->timestap[2]*NOW_TIME_TICK;

        double tau1 = t2 - t3;
        double tau2 = t1 - t2; 
        mainLogPrintf("\n[+]solved t: %.8f, %.8f, %.8f s\n    tau: %.8f, %.8f",t1,t2,t3,tau1,tau2);
        double v = 340.0;   // ���� m/s
        double d = 0.5;       // ���ڴ�����֮��ľ��� m
        double theta, gamma;
        gamma = (2*pow(d,2) - pow(tau2*v,2) + pow(tau1*v,2) - 2*tau1*tau2*pow(v,2))
                /(2*(tau2-tau1)*v);
        gamma = gamma + tau1*v;
        theta = PI/2 - acos( (pow(gamma,2) + pow(d,2) - pow(gamma+tau2*v,2))
                /(2*(gamma)*d) );
        theta = theta * 180.0 / PI;
        mainLogPrintf("\n   [!]theta: %f deg; gamma: %f m",theta,gamma);
        
        if ( -60 < theta && theta < 60 /* && 0 < gamma && gamma < 5*/) {
            PWM_SetDegree(-theta);
        }
        
        
    } else {
        mainLogPrintf(".%x.",ed->data_writed);
    }
//    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
//    HAL_NVIC_EnableIRQ(EXTI1_IRQn);
//    HAL_NVIC_EnableIRQ(EXTI2_IRQn);
    //����
    while (1) {
        GUI_Delay(1);
        if (ed->data_writed == 0x0U) {
            break;
        } else {
            ed->data_writed = 0x0U;
        }
    }
    
}
void voiceSpeed_solve(struct exti_data *ed) {
    mainLogPrintf("\nmain_solve");
    if (ed->data_writed == 0x0001U+0x0002U) {
        double t1 = ed->timestap[0]*NOW_TIME_TICK;
        double t2 = ed->timestap[1]*NOW_TIME_TICK;
        mainLogPrintf("\n   t: %f, %f s",t1,t2);
        mainLogPrintf("\n   d: %f m",fabs(t1-t2)*340.0);
    } else {
        mainLogPrintf("\n   nodata: ed->data_writed=%x",ed->data_writed);
    }
    ed->data_writed = 0;
}

int main(void)
{
    HAL_Init();                   	//��ʼ��HAL��  
    
    Stm32_Clock_Init(336,8,2,7);  	//����ʱ��,168Mhz
	delay_init(168);                //��ʼ����ʱ����
	uart_init(9600);                //��ʼ��USART
    
    TIM3_Init(999,83); 	            //1KHZ ��ʱ��3����Ϊ1ms
    TIM4_Init(999,839);             //������ɨ���ٶ�,100HZ.
    //TIM5_Init(9,83);                //nowTime 100kHZ 0.000 01s.  NOW_TIME_TICK
    TIM14_PWM_Init(5000-1,84-1);    //84M/84=1M�ļ���Ƶ�ʣ��Զ���װ��Ϊ5000����ôPWMƵ��Ϊ1M/5000=200HZ

	LED_Init();						//��ʼ��LED	
    KEY_Init();                     //��ʼ������
	EXTI_Init();                    //�ⲿ�жϳ�ʼ��
	TFTLCD_Init();           	    //��ʼ��LCD FSMC�ӿ�
    TP_Init();				        //��������ʼ��
    
	
	my_mem_init(SRAMIN);			//��ʼ���ڲ��ڴ��
	//my_mem_init(SRAMEX);			//��ʹ���ⲿ�ڴ��
	my_mem_init(SRAMCCM);			//��ʼ��CCM�ڴ��
    
    __HAL_RCC_CRC_CLK_ENABLE();//ʹ��CRCʱ�ӣ�����STemWin����ʹ��

	WM_SetCreateFlags(WM_CF_MEMDEV);//Ϊ�ػ�����Զ�ʹ�ô洢�豸
	GUI_Init();
    GUI_CURSOR_Show();

    //����������
    WM_HWIN CreatemainFramewin(void);
    CreatemainFramewin();
    
    LASER(GPIO_PIN_SET);
    PWM_SetDegree(-90);
    GUI_Delay(2000);
    PWM_SetDegree(90);  
    GUI_Delay(2000);
    PWM_SetDegree(0);   
    LASER(GPIO_PIN_RESET);   
    mainLogPrint("\ninit OK!");

    while(1)
	{
		GUI_Delay(1000);

        main_solve(&ExtiData); 
        //voiceSpeed_solve(&ExtiData); 
        show_nowTime(SysTick->VAL);
        GUI_Exec();
        LED0 = !LED0;
	} 
}
