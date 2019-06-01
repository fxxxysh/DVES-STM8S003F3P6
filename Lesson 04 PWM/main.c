
#include "stm8s.h"
#include "stm8s_clk.h"                                                          // Clock.h
#include "stm8s_gpio.h"                                                         // GPIO.h
#include "stm8s_exti.h"                                                         // External Interrup.h
#include "stm8s_tim1.h"                                                         // Timer1.h
#include "stm8s_tim2.h"                                                         // Timer1.h

/*
//
//              TIM1 Channel duty cycle = TIM_CCR1/ ( TIM_Period + 1)
//
*/

#define CCR1_Val  ((uint16_t)4095)      // TIM1 Channe12 duty cycle = 50%
#define CCR2_Val  ((uint16_t)1535)      // TIM1 Channe12 duty cycle = 37.5%
#define CCR3_Val  ((uint16_t)4095)      // TIM1 Channe12 duty cycle = 25% - 1023
#define CCR4_Val  ((uint16_t)4095)      // TIM1 Channe12 duty cycle = 100% - 4095

uint16_t PWM_Value=0;
uint16_t duty=40;

void Delay_us(uint32_t time_delay);                                             // Ham delay us
void Delay_ms(uint32_t time_delay);                                             // Ham delay ms
void CLK_Cofiguration(void);                                                    // Ham cau hinh cho CLOCK su dung thach anh noi HSI
void GPIO_Configuration(void);                                                  // Ham cau hinh cho chan GPIO
void EXIT_Configuration(void);                                                  // Ham cai hinh ngat ngoai tai chan RA3
void TIMER1_Configuration(void);                                                // Ham cau hinh cho Timer1
void TIMER2_Configuration(void);                                                // Ham cau hinh cho Timer1
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void main(void)
{
  
  CLK_Cofiguration();                                                           // Ham cau hinh CLOCK
  GPIO_Configuration();                                                         // Ham cau hinh GPIO
  EXIT_Configuration();                                                         // Ham cai hinh EXIT
  TIMER2_Configuration();                                                       // hAM cau hinh cho TIMER2
  TIMER1_Configuration();
  
  enableInterrupts();                                                           // Cho Phep NGAT TOAN CUC xay ra ngat
  /* Infinite loop */
  while (1)
  {
   
  
    //04 ham duoi day dung de chinh dinh PWM ra 04 channel cua TIMER1, Channel 1,2,3 co A va -A, con lai channel 4 chi co A, tong cong la 7 PWM, xem datasheet de biet ro chan ngo ra
    
    TIM1_OC1Init(TIM1_OCMODE_PWM2,TIM1_OUTPUTSTATE_ENABLE,TIM1_OUTPUTNSTATE_ENABLE,PWM_Value,TIM1_OCPOLARITY_LOW,TIM1_OCNPOLARITY_HIGH,TIM1_OCIDLESTATE_SET,TIM1_OCNIDLESTATE_RESET); 
    TIM1_OC2Init(TIM1_OCMODE_PWM2,TIM1_OUTPUTSTATE_ENABLE,TIM1_OUTPUTNSTATE_ENABLE,PWM_Value,TIM1_OCPOLARITY_LOW,TIM1_OCNPOLARITY_HIGH,TIM1_OCIDLESTATE_SET, TIM1_OCNIDLESTATE_RESET);
    TIM1_OC3Init(TIM1_OCMODE_PWM2,TIM1_OUTPUTSTATE_ENABLE,TIM1_OUTPUTNSTATE_ENABLE,4095-PWM_Value,TIM1_OCPOLARITY_LOW,TIM1_OCNPOLARITY_HIGH,TIM1_OCIDLESTATE_SET,TIM1_OCNIDLESTATE_RESET);
    TIM1_OC4Init(TIM1_OCMODE_PWM2,TIM1_OUTPUTSTATE_ENABLE,PWM_Value,TIM1_OCPOLARITY_LOW,TIM1_OCIDLESTATE_SET);
    
    Delay_ms(10); 
    
    PWM_Value += duty;
    if(PWM_Value<=0||PWM_Value>=4095)
      {
        duty = -duty; //neu STEP=0 or 1024 thi se dao duty lai
      }
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLK_Cofiguration(void)
{
//  CLK_DeInit();								   // reset tat ca cac nguon clock
//  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);                                // Cau hinh clock HSI voi ty le chia 1 - 16Mhz/1=16Mhz
  CLK_DeInit();
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
  CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
}
void GPIO_Configuration(void)
{
  GPIO_DeInit(GPIOD);                                                           // reset trang thai cho tat ca port D
  GPIO_DeInit(GPIOA);                                                           // reset trang thai cho tat ca port D
  GPIO_Init(GPIOD,GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST);                       // cau hinh chan PD2 cua portD voi mode push pull toc do 10Mhz, Muc 0
  GPIO_Init(GPIOD,GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST);                      // cau hinh chan PD3 cua portD voi mode push pull toc do 10Mhz, Muc 1
  GPIO_Init(GPIOA,GPIO_PIN_3, GPIO_MODE_IN_PU_IT); 	                        // cau hinh chan PA3 la chan input, dien tro keo len, co ngat
}
void EXIT_Configuration(void)
{
  EXTI_DeInit();
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOA,EXTI_SENSITIVITY_FALL_ONLY);        //cau hinh ngat o cac chan GPIOA(A3), xay ra ngat khi co xung canh xuong

}
void TIMER1_Configuration(void)
{
   TIM1_DeInit();                                                               // reset trang thai cho tat ca TIM1                

  /* Time Base configuration */
  /*
  TIM1_Period = 4095
  TIM1_Prescaler = 0
  TIM1_CounterMode = TIM1_COUNTERMODE_UP
  TIM1_RepetitionCounter = 0
  */

  TIM1_TimeBaseInit(0, TIM1_COUNTERMODE_UP, 4095, 0);                           // cau hinh time1 la che do dem len

  /* Channel 1, 2,3 and 4 Configuration in PWM mode */
  
  /*
  TIM1_OCMode = TIM1_OCMODE_PWM2
  TIM1_OutputState = TIM1_OUTPUTSTATE_ENABLE
  TIM1_OutputNState = TIM1_OUTPUTNSTATE_ENABLE
  TIM1_Pulse = CCR1_Val
  TIM1_OCPolarity = TIM1_OCPOLARITY_LOW
  TIM1_OCNPolarity = TIM1_OCNPOLARITY_HIGH
  TIM1_OCIdleState = TIM1_OCIDLESTATE_SET
  TIM1_OCNIdleState = TIM1_OCIDLESTATE_RESET
  */
  
  //04 ham duoi day dung de set PWM cho 4 channel cua TIM1, gia tri se duoc thay vao CCRX_VAL
  
  /*TIM1_Pulse = CCR2_Val*/
    TIM1_OC1Init(TIM1_OCMODE_PWM2,
                TIM1_OUTPUTSTATE_ENABLE,
		TIM1_OUTPUTNSTATE_ENABLE,
		CCR1_Val,
		TIM1_OCPOLARITY_LOW,
		TIM1_OCNPOLARITY_HIGH,
		TIM1_OCIDLESTATE_SET,
		TIM1_OCNIDLESTATE_RESET); 

  /*TIM1_Pulse = CCR2_Val*/
  TIM1_OC2Init(TIM1_OCMODE_PWM2,
		TIM1_OUTPUTSTATE_ENABLE,
		TIM1_OUTPUTNSTATE_ENABLE,
		CCR2_Val,
		TIM1_OCPOLARITY_LOW,
		TIM1_OCNPOLARITY_HIGH,
		TIM1_OCIDLESTATE_SET, 
		TIM1_OCNIDLESTATE_RESET);

  /*TIM1_Pulse = CCR3_Val*/
  TIM1_OC3Init(TIM1_OCMODE_PWM2,
		TIM1_OUTPUTSTATE_ENABLE,
		TIM1_OUTPUTNSTATE_ENABLE,
		CCR3_Val,
		TIM1_OCPOLARITY_LOW,
		TIM1_OCNPOLARITY_HIGH,
		TIM1_OCIDLESTATE_SET,
		TIM1_OCNIDLESTATE_RESET);

  /*TIM1_Pulse = CCR4_Val*/
  TIM1_OC4Init(TIM1_OCMODE_PWM2,
		TIM1_OUTPUTSTATE_ENABLE,
		CCR4_Val,
		TIM1_OCPOLARITY_LOW,
		TIM1_OCIDLESTATE_SET);

  /* TIM1 counter enable */
  TIM1_Cmd(ENABLE);

  /* TIM1 Main Output Enable */
  TIM1_CtrlPWMOutputs(ENABLE);
  
    /* 
    - Connect TIM1 pins to an oscilloscope as follows:
     - TIM1_CH1  pin (PC.1)  
     - TIM1_CH1N pin (PH.7)  
     - TIM1_CH2  pin (PC.2)  
     - TIM1_CH2N pin (PH.6)  
     - TIM1_CH3  pin (PC.3)  
     - TIM1_CH3N pin (PH.5)  
     - TIM1_CH4 pin  (PC.4)
  */
}
void TIMER2_Configuration(void)                                                 // Ham cau hinh cho TIM2
{
  TIM2_DeInit();
  TIM2_TimeBaseInit(TIM2_PRESCALER_32768,243);                                   // cau hinh bo chia cho TIMER2 chay o 0.5(s)  
  TIM2_ClearFlag(TIM2_FLAG_UPDATE);                                              // Xoa co ngat timer2
  TIM2_ITConfig(TIM2_IT_UPDATE,ENABLE);                                          // Cho phep ngat khi tran sau khi dem 487 cycle
  TIM2_Cmd(ENABLE);
}

void Delay_us(uint32_t time_delay)
{
  while(time_delay--)
  {
  }
}

void Delay_ms(uint32_t time_delay)
{
  while(time_delay--)
  {
    Delay_us(200);
  }
}

















#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
