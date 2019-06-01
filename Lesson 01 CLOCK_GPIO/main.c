
#include "stm8s.h"
#include "stm8s_clk.h"
#include "stm8s_gpio.h"

void Delay_us(uint32_t time_delay);                                             // Ham delay us
void Delay_ms(uint32_t time_delay);                                             // Ham delay ms
void CLK_Cofiguration(void);                                                    // Ham cau hinh cho CLOCK su dung thach anh noi HSI
void GPIO_Configuration(void);                                                  // Ham cau hinh cho chan GPIO

void main(void)
{
  
  CLK_Cofiguration();                                                           // Ham cau hinh CLOCK
  GPIO_Configuration();                                                         // Ham cau hinh GPIO
  
  /* Infinite loop */
  while (1)
  {
    for(int i=0;i<=5;i++)
    {
      GPIO_WriteReverse(GPIOD,GPIO_PIN_2);                                       //dao muc logic o chan PD2
      GPIO_WriteReverse(GPIOD,GPIO_PIN_3);		                        //dao muc logic o chan PD3
      Delay_ms(1000);
    }
    for(int i=0;i<=10;i++)
    {
      GPIO_Write(GPIOD,0x00);                                                   // Logic 0 o tat ca cac chan GPIOD (0->7)
      Delay_ms(250); 
      GPIO_Write(GPIOD,0xff);                                                  // Logic 1 o tat ca cac chan GPIOD (0->7)
      Delay_ms(250);                                                           // ham delay tuong doi 1s
    }
  }
}

void CLK_Cofiguration(void)
{
  CLK_DeInit();								        // reset tat ca cac nguon clock
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);                                // Cau hinh clock HSI voi ty le chia 1 - 16Mhz/1=16Mhz
}

void GPIO_Configuration(void)
{
  GPIO_DeInit(GPIOD);                                                           // reset trang thai cho tat ca port D
  GPIO_DeInit(GPIOA);                                                           // reset trang thai cho tat ca port D
  GPIO_Init(GPIOD,GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST);                       // cau hinh chan PD2 cua portD voi mode push pull toc do 10Mhz, Muc 0
  GPIO_Init(GPIOD,GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST);                      // cau hinh chan PD3 cua portD voi mode push pull toc do 10Mhz, Muc 1
  GPIO_Init(GPIOA,GPIO_PIN_3, GPIO_MODE_IN_PU_NO_IT); 	                        // cau hinh chan PA3 la chan input, dien tro keo len, k ngat
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
