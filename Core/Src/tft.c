#include "tft.h"

/* ═══════════════════════════════════════════════════
   TFT_Init
   Call once in main USER CODE BEGIN 2
   Handles reset + register init + backlight
═══════════════════════════════════════════════════ */
void TFT_Init(void)
{
    ILI9341_Init();          /* reset + full register init + BLK on */
    TFT_Clear(Black);        /* start with clean black screen        */
}

/* ═══════════════════════════════════════════════════
   Screen helpers
═══════════════════════════════════════════════════ */
void TFT_Clear(uint16_t color)
{
    ILI9341_FillScreen(color);
}

void TFT_BacklightOn(void)
{
    ILI9341_BLK_ON();
}

void TFT_BacklightOff(void)
{
    ILI9341_BLK_OFF();
}

/* ═══════════════════════════════════════════════════
   TFT_DrawChar
   Renders one character from a FontDef_t bitmap.

   Font data layout (same as your OLED fonts.c):
   Each character = FontHeight rows of uint16_t
   Each uint16_t row = FontWidth bits, MSB first
   Bit set   = draw foreground color
   Bit clear = draw background color
═══════════════════════════════════════════════════ */
void TFT_DrawChar(uint16_t x, uint16_t y,
                  uint16_t color, uint16_t bg,
                  FontDef_t *font, char ch)
{
    if (x + font->FontWidth  > ILI9341_WIDTH)  return;
    if (y + font->FontHeight > ILI9341_HEIGHT) return;

    /* move to printable ASCII range */
    uint32_t char_offset = (uint32_t)(ch - 32) * font->FontHeight;

    ILI9341_SetWindow(x, y,
                      x + font->FontWidth  - 1,
                      y + font->FontHeight - 1);

    for (uint8_t row = 0; row < font->FontHeight; row++) {
        uint16_t bits = font->data[char_offset + row];
        for (uint8_t col = 0; col < font->FontWidth; col++) {
            /* MSB is leftmost pixel */
            if (bits & (0x8000 >> col))
                ILI9341_WriteData16(color);
            else
                ILI9341_WriteData16(bg);
        }
    }
}

/* ═══════════════════════════════════════════════════
   TFT_PrintFont — full control
   Renders a string, wraps at screen edge
═══════════════════════════════════════════════════ */
void TFT_PrintFont(uint16_t x, uint16_t y,
                   uint16_t color, uint16_t bg,
                   FontDef_t *font, const char *str)
{
    uint16_t cx = x;
    while (*str) {
        if (cx + font->FontWidth > ILI9341_WIDTH) {
            cx  = x;
            y  += font->FontHeight + 2;
        }
        if (y + font->FontHeight > ILI9341_HEIGHT) break;
        TFT_DrawChar(cx, y, color, bg, font, *str);
        cx += font->FontWidth;
        str++;
    }
}

/* ═══════════════════════════════════════════════════
   Convenience wrappers — bg always Black
   Match your requested syntax:
     TFT_Print(0, 0, White, "Ready");
═══════════════════════════════════════════════════ */
void TFT_Print(uint16_t x, uint16_t y,
               uint16_t color, const char *str)
{
    TFT_PrintFont(x, y, color, Black, &Font_7x10, str);
}

void TFT_PrintMed(uint16_t x, uint16_t y,
                  uint16_t color, const char *str)
{
    TFT_PrintFont(x, y, color, Black, &Font_11x18, str);
}

void TFT_PrintLarge(uint16_t x, uint16_t y,
                    uint16_t color, const char *str)
{
    TFT_PrintFont(x, y, color, Black, &Font_16x26, str);
}

/* ═══════════════════════════════════════════════════
   Shape wrappers — pass through to ili9341
═══════════════════════════════════════════════════ */
void TFT_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    ILI9341_DrawPixel(x, y, color);
}

void TFT_FillRect(uint16_t x, uint16_t y,
                  uint16_t w, uint16_t h, uint16_t color)
{
    ILI9341_FillRect(x, y, w, h, color);
}

void TFT_DrawRect(uint16_t x, uint16_t y,
                  uint16_t w, uint16_t h, uint16_t color)
{
    ILI9341_DrawRect(x, y, w, h, color);
}

void TFT_DrawLine(uint16_t x0, uint16_t y0,
                  uint16_t x1, uint16_t y1, uint16_t color)
{
    ILI9341_DrawLine(x0, y0, x1, y1, color);
}

void TFT_DrawCircle(uint16_t x, uint16_t y,
                    uint16_t r, uint16_t color)
{
    ILI9341_DrawCircle(x, y, r, color);
}

void TFT_FillCircle(uint16_t x, uint16_t y,
                    uint16_t r, uint16_t color)
{
    ILI9341_FillCircle(x, y, r, color);
}
