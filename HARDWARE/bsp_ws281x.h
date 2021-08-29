#ifndef __BSP_WS281X_H
#define __BSP_WS281X_H

#include "main.h"
#include "tim.h"

#define PIXEL_NUM 30
#define GRB  24   //3*8

#define WS_HIGH 45
#define WS_LOW  30

void TM7812_show(void);
void WS281x_Init(void);
void WS281x_CloseAll(void);
uint32_t ws281x_color(uint8_t red, uint8_t green, uint8_t blue);
void ws281x_setPixelColor(uint16_t n, uint32_t GRBColor);
void ws281x_setPixelRGB(uint16_t n ,uint8_t red, uint8_t green, uint8_t blue);
void ws281x_show(void);

void ws281x_rainbowCycle(uint8_t wait);
void ws281x_theaterChase(uint32_t c, uint8_t wait);
void ws281x_colorWipe(uint32_t c, uint8_t wait);
void ws281x_rainbow(uint8_t wait);
void ws281x_theaterChaseRainbow(uint8_t wait);

void All_Light_LED(uint32_t c, uint8_t wait);
void One_PieceLight_LED(uint32_t c, uint8_t wait);
void Two_PiecesLight_LED(uint32_t c, uint8_t wait);
void Three_PiecesLight_LED(uint32_t c, uint8_t wait);
void Four_PiecesLight_LED(uint32_t c, uint8_t wait);

void FlashingLight_LED(uint32_t c, uint8_t wait);

#endif /* __BSP_WS281X_H */

