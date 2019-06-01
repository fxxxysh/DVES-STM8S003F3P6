
#include "stm8s.h"
#include "stm8s_clk.h"                                                          // Clock.h
#include "stm8s_gpio.h"                                                         // GPIO.h
#include "stm8s_exti.h"                                                         // External Interrup.h
#include "stm8s_tim1.h"                                                         // Timer1.h
#include "stm8s_tim2.h"                                                         // Timer1.h


void Delay_us(uint32_t time_delay);                                             // Ham delay us
void Delay_ms(uint32_t time_delay);                                             // Ham delay ms
void CLK_Cofiguration(void);                                                    // Ham cau hinh cho CLOCK su dung thach anh noi HSI
void GPIO_Configuration(void);                                                  // Ham cau hinh cho chan GPIO
void EXIT_Configuration(void);                                                  // Ham cai hinh ngat ngoai tai chan RA3
void TIMER1_Configuration(void);                                                // Ham cau hinh cho Timer1
void TIMER2_Configuration(void);                                                // Ham cau hinh cho Timer1

void main(void)
{
  
  CLK_Cofiguration();                                                           // Ham cau hinh CLOCK
  GPIO_Configuration();                                                         // Ham cau hinh GPIO
  EXIT_Configuration();                                                         // Ham cai hinh EXIT
  TIMER2_Configuration();                                                       // hAM cau hinh cho TIMER2
  
  enableInterrupts();                                                           // Cho Phep NGAT TOAN CUC xay ra ngat
  /* Infinite loop */
  while (1)
  {
  }
}

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
  TIM1_DeInit();
//  TIM1_TimeBaseInit(TIM1_PRESCALER_512,
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
