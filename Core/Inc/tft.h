#ifndef TFT_H
#define TFT_H

#include "main.h"
#include "ili9341.h"
#include "fonts.h"

/* ═══════════════════════════════════════════════════
   Convenience color aliases  (same names as your OLED
   uses for SSD1306_COLOR_WHITE / BLACK pattern)
═══════════════════════════════════════════════════ */
#define Black       ILI9341_BLACK
#define White       ILI9341_WHITE
#define Red         ILI9341_RED
#define Green       ILI9341_GREEN
#define Blue        ILI9341_BLUE
#define Yellow      ILI9341_YELLOW
#define Cyan        ILI9341_CYAN
#define Magenta     ILI9341_MAGENTA
#define Orange      ILI9341_ORANGE
#define Purple      ILI9341_PURPLE
#define Gray        ILI9341_GRAY
#define LightGray   ILI9341_LIGHTGRAY
#define DarkGray    ILI9341_DARKGRAY

/* ═══════════════════════════════════════════════════
   Startup
═══════════════════════════════════════════════════ */
void TFT_Init(void);         /* full startup — call once in main */

/* ═══════════════════════════════════════════════════
   Screen
═══════════════════════════════════════════════════ */
void TFT_Clear(uint16_t color);           /* fill whole screen    */
void TFT_BacklightOn(void);
void TFT_BacklightOff(void);

/* ═══════════════════════════════════════════════════
   Text
   TFT_Print     — default small font (Font_7x10), bg = Black
   TFT_PrintMed  — medium font (Font_11x18),       bg = Black
   TFT_PrintLarge— large font  (Font_16x26),       bg = Black
   TFT_PrintFont — full control: specify font + bg color
═══════════════════════════════════════════════════ */
void TFT_Print(uint16_t x, uint16_t y,
               uint16_t color, const char *str);

void TFT_PrintMed(uint16_t x, uint16_t y,
                  uint16_t color, const char *str);

void TFT_PrintLarge(uint16_t x, uint16_t y,
                    uint16_t color, const char *str);

void TFT_PrintFont(uint16_t x, uint16_t y,
                   uint16_t color, uint16_t bg,
                   FontDef_t *font, const char *str);

void TFT_DrawChar(uint16_t x, uint16_t y,
                  uint16_t color, uint16_t bg,
                  FontDef_t *font, char ch);

/* ═══════════════════════════════════════════════════
   Shapes
═══════════════════════════════════════════════════ */
void TFT_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void TFT_FillRect(uint16_t x, uint16_t y,
                  uint16_t w, uint16_t h, uint16_t color);
void TFT_DrawRect(uint16_t x, uint16_t y,
                  uint16_t w, uint16_t h, uint16_t color);
void TFT_DrawLine(uint16_t x0, uint16_t y0,
                  uint16_t x1, uint16_t y1, uint16_t color);
void TFT_DrawCircle(uint16_t x, uint16_t y,
                    uint16_t r, uint16_t color);
void TFT_FillCircle(uint16_t x, uint16_t y,
                    uint16_t r, uint16_t color);

#endif /* TFT_H */
