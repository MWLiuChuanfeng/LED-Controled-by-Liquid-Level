#include "bsp_ws281x.h"
#include <string.h>

uint16_t pixelBuffer[PIXEL_NUM][GRB];


void WS281x_Init(void)
{
	//MX_TIM2_Init();
  /*关闭所有灯珠*/
  WS281x_CloseAll();
  HAL_Delay(100);
}

void WS281x_CloseAll(void)
{
  uint16_t i;
  uint8_t j;
  
  for(i = 0; i < PIXEL_NUM; ++i)
  {
    for(j = 0; j < 24; ++j)
    {
      pixelBuffer[i][j] = WS_LOW;
    }
  }
  ws281x_show(); 
}


/**
  * @brief  将RGB颜色组合到一个32位数据中存储
  * @param  red：  0-255
  *         green：0-255
  *         blue： 0-255
  * @retval None
  */
uint32_t ws281x_color(uint8_t red, uint8_t green, uint8_t blue)
{
  return (uint32_t)(green << 16 | red << 8 | blue);
}

/**
  * @brief  给特定LED灯设定颜色
  * @param  ｎ：LED灯号
　*         GRBClor: 32位的颜色值
  * @retval None
  */
void ws281x_setPixelColor(uint16_t n, uint32_t GRBColor)
{
  uint8_t i;
  if(n < PIXEL_NUM)
  {
    for(i = 0; i < GRB; i++)
    {
      pixelBuffer[n][i] = ((GRBColor << i) & 0x800000) ? WS_HIGH : WS_LOW;
    }
  }
}

void ws281x_setPixelRGB(uint16_t n ,uint8_t red, uint8_t green, uint8_t blue)
{
  uint8_t i;
  
  if(n < PIXEL_NUM)
  {
    for(i = 0; i < GRB; ++i)
    {
      pixelBuffer[n][i] = (((ws281x_color(red,green,blue) << i) & 0X800000) ? WS_HIGH : WS_LOW);
    }
  }
}

//下面函数中语句执行的顺序相当关键，不能错。
//从示波器看，非当前的执行顺序，最后一个灯的波形序列会多。
void ws281x_show(void)
{
  HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t *)&pixelBuffer, (uint16_t)(PIXEL_NUM * GRB)); 
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t ws281x_wheel(uint8_t wheelPos) {
  wheelPos = 255 - wheelPos;
  if(wheelPos < 85) {
    return ws281x_color(255 - wheelPos * 3, 0, wheelPos * 3);
  }
  if(wheelPos < 170) {
    wheelPos -= 85;
    return ws281x_color(0, wheelPos * 3, 255 - wheelPos * 3);
  }
  wheelPos -= 170;
  return ws281x_color(wheelPos * 3, 255 - wheelPos * 3, 0);
}

// Fill the dots one after the other with a color
void ws281x_colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<PIXEL_NUM; i++) {
    ws281x_setPixelColor(i, c);
    ws281x_show();
    HAL_Delay(wait);
  }
}

void ws281x_rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<PIXEL_NUM; i++) {
      ws281x_setPixelColor(i, ws281x_wheel((i+j) & 255));
    }
    ws281x_show();
    HAL_Delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void ws281x_rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< PIXEL_NUM; i++) {
      ws281x_setPixelColor(i,ws281x_wheel(((i * 256 / PIXEL_NUM) + j) & 255));
    }
    ws281x_show();
    HAL_Delay(wait);
  }
}

//Theatre-style crawling lights.
void ws281x_theaterChase(uint32_t c, uint8_t wait) 
	{
  for (int j=0; j<10; j++)
	{  //do 10 cycles of chasing
    for (int q=0; q < 3; q++)
		{
      for (uint16_t i=0; i < PIXEL_NUM; i=i+3) 
			{
        ws281x_setPixelColor(i+q, c);    //turn every third pixel on
      }
      ws281x_show();

      HAL_Delay(wait);

      for (uint16_t i=0; i < PIXEL_NUM; i=i+3) 
			{
        ws281x_setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void ws281x_theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < PIXEL_NUM; i=i+3) {
        ws281x_setPixelColor(i+q, ws281x_wheel( (i+j) % 255));    //turn every third pixel on
      }
      ws281x_show();

      HAL_Delay(wait);

      for (uint16_t i=0; i < PIXEL_NUM; i=i+3) {
        ws281x_setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}


/* 中断回调函数，在设定的pwm通过DMA发送完成后会调用 */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef* htim)
{
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0); //占空比清0，若不清会导致灯珠颜色不对 
  HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_1);
}




/* 点亮所有灯珠 */
void All_Light_LED(uint32_t c, uint8_t wait) {
  for(uint16_t i=0 ; i<PIXEL_NUM; i++) {
    ws281x_setPixelColor(i, c);
    ws281x_show();
    HAL_Delay(wait);
  }
}


/* 一段灯珠常亮 */
void One_PieceLight_LED(uint32_t c, uint8_t wait)
{

			 for(uint16_t i=0 ; i<PIXEL_NUM/5; i++) {
				ws281x_setPixelColor(i, c);
				ws281x_show();
				HAL_Delay(wait);
			}
}


/* 两段灯珠常亮 */
void Two_PiecesLight_LED(uint32_t c, uint8_t wait) {
  for(uint16_t i=0 ; i<PIXEL_NUM/5*2; i++) {
    ws281x_setPixelColor(i, c);
    ws281x_show();
    HAL_Delay(wait);
  }
}


/* 三段灯珠常亮 */
void Three_PiecesLight_LED(uint32_t c, uint8_t wait) {
  for(uint16_t i=0 ; i<PIXEL_NUM/5*3; i++) {
    ws281x_setPixelColor(i, c);
    ws281x_show();
    HAL_Delay(wait);
  }
}

/* 四段灯珠常亮 */
void Four_PiecesLight_LED(uint32_t c, uint8_t wait) {
  for(uint16_t i=0 ; i<PIXEL_NUM/5*4; i++) {
    ws281x_setPixelColor(i, c);
    ws281x_show();
    HAL_Delay(wait);
  }
}


/* 最后一段灯珠闪烁 */
void FlashingLight_LED(uint32_t c, uint8_t wait) 
{
	  for(int i=0;i<50;i++)
			{
				WS281x_CloseAll();
				One_PieceLight_LED(c,wait);
				
			}
	
}




