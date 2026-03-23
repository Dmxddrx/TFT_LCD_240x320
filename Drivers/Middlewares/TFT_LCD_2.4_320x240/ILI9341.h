#ifndef ILI9341_H
#define ILI9341_H

#include "main.h"

/* ═══════════════════════════════════════════════════
   FSMC bus addresses  —  8-bit bus, RS on FSMC_A18
   CMD  address: A18 = 0  →  RS LOW  = command
   DATA address: A18 = 1  →  RS HIGH = data
═══════════════════════════════════════════════════ */
#define ILI9341_CMD_ADDR   (*(__IO uint8_t *)(0x60000000))
#define ILI9341_DATA_ADDR  (*(__IO uint8_t *)(0x60040000))

/* ═══════════════════════════════════════════════════
   Hardware control pins
═══════════════════════════════════════════════════ */
#define ILI9341_RST_LOW()  HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET)
#define ILI9341_RST_HIGH() HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET)
#define ILI9341_BLK_ON()   HAL_GPIO_WritePin(LCD_BLK_GPIO_Port, LCD_BLK_Pin, GPIO_PIN_SET)
#define ILI9341_BLK_OFF()  HAL_GPIO_WritePin(LCD_BLK_GPIO_Port, LCD_BLK_Pin, GPIO_PIN_RESET)

/* ═══════════════════════════════════════════════════
   Screen dimensions
═══════════════════════════════════════════════════ */
#define ILI9341_WIDTH   240
#define ILI9341_HEIGHT  320

/* ═══════════════════════════════════════════════════
   RGB565 color table
   Convert: R(5bit) G(6bit) B(5bit)
═══════════════════════════════════════════════════ */
#define ILI9341_BLACK       0x0000
#define ILI9341_WHITE       0xFFFF
#define ILI9341_RED         0xF800
#define ILI9341_GREEN       0x07E0
#define ILI9341_BLUE        0x001F
#define ILI9341_YELLOW      0xFFE0
#define ILI9341_CYAN        0x07FF
#define ILI9341_MAGENTA     0xF81F
#define ILI9341_ORANGE      0xFD20
#define ILI9341_PURPLE      0x8010
#define ILI9341_GRAY        0x8410
#define ILI9341_LIGHTGRAY   0xC618
#define ILI9341_DARKGRAY    0x4208
#define ILI9341_NAVY        0x000F
#define ILI9341_DARKGREEN   0x03E0
#define ILI9341_MAROON      0x7800
#define ILI9341_OLIVE       0x7BE0
#define ILI9341_TEAL        0x03EF

/* ═══════════════════════════════════════════════════
   Low-level write functions
═══════════════════════════════════════════════════ */
void ILI9341_WriteCmd(uint8_t cmd);
void ILI9341_WriteData(uint8_t data);
void ILI9341_WriteData16(uint16_t data);
void ILI9341_SetWindow(uint16_t x0, uint16_t y0,
                       uint16_t x1, uint16_t y1);

/* ═══════════════════════════════════════════════════
   Core functions
═══════════════════════════════════════════════════ */
void ILI9341_Init(void);
void ILI9341_FillScreen(uint16_t color);
void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void ILI9341_FillRect(uint16_t x, uint16_t y,
                      uint16_t w, uint16_t h, uint16_t color);
void ILI9341_DrawRect(uint16_t x, uint16_t y,
                      uint16_t w, uint16_t h, uint16_t color);
void ILI9341_DrawLine(uint16_t x0, uint16_t y0,
                      uint16_t x1, uint16_t y1, uint16_t color);
void ILI9341_DrawCircle(uint16_t cx, uint16_t cy,
                        uint16_t r,  uint16_t color);
void ILI9341_FillCircle(uint16_t cx, uint16_t cy,
                        uint16_t r,  uint16_t color);

#endif /* ILI9341_H */
