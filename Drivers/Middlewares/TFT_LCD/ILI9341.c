#include "ili9341.h"

/* ═══════════════════════════════════════════════════
   Low-level bus write
═══════════════════════════════════════════════════ */
void ILI9341_WriteCmd(uint8_t cmd)
{
    ILI9341_CMD_ADDR = cmd;
}

void ILI9341_WriteData(uint8_t data)
{
    ILI9341_DATA_ADDR = data;
}

void ILI9341_WriteData16(uint16_t data)
{
    ILI9341_DATA_ADDR = (uint8_t)(data >> 8);    /* high byte first */
    ILI9341_DATA_ADDR = (uint8_t)(data & 0xFF);  /* low byte second */
}

/* ═══════════════════════════════════════════════════
   Set drawing window on ILI9341
   All pixel writes after this go into this rectangle
═══════════════════════════════════════════════════ */
void ILI9341_SetWindow(uint16_t x0, uint16_t y0,
                       uint16_t x1, uint16_t y1)
{
    ILI9341_WriteCmd(0x2A);          /* column address set */
    ILI9341_WriteData16(x0);
    ILI9341_WriteData16(x1);

    ILI9341_WriteCmd(0x2B);          /* row address set */
    ILI9341_WriteData16(y0);
    ILI9341_WriteData16(y1);

    ILI9341_WriteCmd(0x2C);          /* memory write — pixel data follows */
}

/* ═══════════════════════════════════════════════════
   ILI9341_Init
   Full register init sequence for ILI9341
   Must be called after hardware reset
═══════════════════════════════════════════════════ */
void ILI9341_Init(void)
{
    /* hardware reset */
    ILI9341_RST_LOW();
    HAL_Delay(20);
    ILI9341_RST_HIGH();
    HAL_Delay(120);

    /* software reset */
    ILI9341_WriteCmd(0x01);
    HAL_Delay(100);

    /* power control A */
    ILI9341_WriteCmd(0xCB);
    ILI9341_WriteData(0x39);
    ILI9341_WriteData(0x2C);
    ILI9341_WriteData(0x00);
    ILI9341_WriteData(0x34);
    ILI9341_WriteData(0x02);

    /* power control B */
    ILI9341_WriteCmd(0xCF);
    ILI9341_WriteData(0x00);
    ILI9341_WriteData(0xC1);
    ILI9341_WriteData(0x30);

    /* driver timing control A */
    ILI9341_WriteCmd(0xE8);
    ILI9341_WriteData(0x85);
    ILI9341_WriteData(0x00);
    ILI9341_WriteData(0x78);

    /* driver timing control B */
    ILI9341_WriteCmd(0xEA);
    ILI9341_WriteData(0x00);
    ILI9341_WriteData(0x00);

    /* power on sequence control */
    ILI9341_WriteCmd(0xED);
    ILI9341_WriteData(0x64);
    ILI9341_WriteData(0x03);
    ILI9341_WriteData(0x12);
    ILI9341_WriteData(0x81);

    /* pump ratio control */
    ILI9341_WriteCmd(0xF7);
    ILI9341_WriteData(0x20);

    /* power control 1 */
    ILI9341_WriteCmd(0xC0);
    ILI9341_WriteData(0x23);

    /* power control 2 */
    ILI9341_WriteCmd(0xC1);
    ILI9341_WriteData(0x10);

    /* VCOM control 1 */
    ILI9341_WriteCmd(0xC5);
    ILI9341_WriteData(0x3E);
    ILI9341_WriteData(0x28);

    /* VCOM control 2 */
    ILI9341_WriteCmd(0xC7);
    ILI9341_WriteData(0x86);

    /* memory access control — portrait mode, RGB order */
    ILI9341_WriteCmd(0x36);
    ILI9341_WriteData(0x48);

    /* pixel format — RGB565 (16 bit) */
    ILI9341_WriteCmd(0x3A);
    ILI9341_WriteData(0x55);

    /* frame rate — 70Hz */
    ILI9341_WriteCmd(0xB1);
    ILI9341_WriteData(0x00);
    ILI9341_WriteData(0x18);

    /* display function control */
    ILI9341_WriteCmd(0xB6);
    ILI9341_WriteData(0x08);
    ILI9341_WriteData(0x82);
    ILI9341_WriteData(0x27);

    /* 3 gamma disable */
    ILI9341_WriteCmd(0xF2);
    ILI9341_WriteData(0x00);

    /* gamma curve select */
    ILI9341_WriteCmd(0x26);
    ILI9341_WriteData(0x01);

    /* positive gamma correction */
    ILI9341_WriteCmd(0xE0);
    ILI9341_WriteData(0x0F); ILI9341_WriteData(0x31);
    ILI9341_WriteData(0x2B); ILI9341_WriteData(0x0C);
    ILI9341_WriteData(0x0E); ILI9341_WriteData(0x08);
    ILI9341_WriteData(0x4E); ILI9341_WriteData(0xF1);
    ILI9341_WriteData(0x37); ILI9341_WriteData(0x07);
    ILI9341_WriteData(0x10); ILI9341_WriteData(0x03);
    ILI9341_WriteData(0x0E); ILI9341_WriteData(0x09);
    ILI9341_WriteData(0x00);

    /* negative gamma correction */
    ILI9341_WriteCmd(0xE1);
    ILI9341_WriteData(0x00); ILI9341_WriteData(0x0E);
    ILI9341_WriteData(0x14); ILI9341_WriteData(0x03);
    ILI9341_WriteData(0x11); ILI9341_WriteData(0x07);
    ILI9341_WriteData(0x31); ILI9341_WriteData(0xC1);
    ILI9341_WriteData(0x48); ILI9341_WriteData(0x08);
    ILI9341_WriteData(0x0F); ILI9341_WriteData(0x0C);
    ILI9341_WriteData(0x31); ILI9341_WriteData(0x36);
    ILI9341_WriteData(0x0F);

    /* exit sleep */
    ILI9341_WriteCmd(0x11);
    HAL_Delay(120);

    /* display on */
    ILI9341_WriteCmd(0x29);
    HAL_Delay(20);

    /* backlight on */
    ILI9341_BLK_ON();
}

/* ═══════════════════════════════════════════════════
   Drawing primitives
═══════════════════════════════════════════════════ */
void ILI9341_FillScreen(uint16_t color)
{
    ILI9341_SetWindow(0, 0, ILI9341_WIDTH - 1, ILI9341_HEIGHT - 1);
    uint32_t total = (uint32_t)ILI9341_WIDTH * ILI9341_HEIGHT;
    for (uint32_t i = 0; i < total; i++)
        ILI9341_WriteData16(color);
}

void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    if (x >= ILI9341_WIDTH || y >= ILI9341_HEIGHT) return;
    ILI9341_SetWindow(x, y, x, y);
    ILI9341_WriteData16(color);
}

void ILI9341_FillRect(uint16_t x, uint16_t y,
                      uint16_t w, uint16_t h, uint16_t color)
{
    if (x + w > ILI9341_WIDTH)  w = ILI9341_WIDTH  - x;
    if (y + h > ILI9341_HEIGHT) h = ILI9341_HEIGHT - y;
    ILI9341_SetWindow(x, y, x + w - 1, y + h - 1);
    uint32_t total = (uint32_t)w * h;
    for (uint32_t i = 0; i < total; i++)
        ILI9341_WriteData16(color);
}

void ILI9341_DrawRect(uint16_t x, uint16_t y,
                      uint16_t w, uint16_t h, uint16_t color)
{
    ILI9341_FillRect(x,         y,         w, 1, color); /* top    */
    ILI9341_FillRect(x,         y + h - 1, w, 1, color); /* bottom */
    ILI9341_FillRect(x,         y,         1, h, color); /* left   */
    ILI9341_FillRect(x + w - 1, y,         1, h, color); /* right  */
}

void ILI9341_DrawLine(uint16_t x0, uint16_t y0,
                      uint16_t x1, uint16_t y1, uint16_t color)
{
    int16_t dx  =  (int16_t)(x1 > x0 ? x1 - x0 : x0 - x1);
    int16_t dy  = -(int16_t)(y1 > y0 ? y1 - y0 : y0 - y1);
    int16_t sx  = x0 < x1 ? 1 : -1;
    int16_t sy  = y0 < y1 ? 1 : -1;
    int16_t err = dx + dy;
    while (1) {
        ILI9341_DrawPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int16_t e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void ILI9341_DrawCircle(uint16_t cx, uint16_t cy,
                        uint16_t r,  uint16_t color)
{
    int16_t x = 0, y = (int16_t)r, d = 1 - (int16_t)r;
    while (x <= y) {
        ILI9341_DrawPixel(cx + x, cy + y, color);
        ILI9341_DrawPixel(cx - x, cy + y, color);
        ILI9341_DrawPixel(cx + x, cy - y, color);
        ILI9341_DrawPixel(cx - x, cy - y, color);
        ILI9341_DrawPixel(cx + y, cy + x, color);
        ILI9341_DrawPixel(cx - y, cy + x, color);
        ILI9341_DrawPixel(cx + y, cy - x, color);
        ILI9341_DrawPixel(cx - y, cy - x, color);
        if (d < 0) d += 2 * x + 3;
        else       { d += 2 * (x - y) + 5; y--; }
        x++;
    }
}

void ILI9341_FillCircle(uint16_t cx, uint16_t cy,
                        uint16_t r,  uint16_t color)
{
    for (int16_t y = -(int16_t)r; y <= (int16_t)r; y++) {
        int16_t xspan = (int16_t)( /* integer sqrt */
            r * r - y * y > 0 ?
            __builtin_sqrt((float)(r * r - y * y)) : 0 );
        ILI9341_FillRect(cx - xspan, cy + y, 2 * xspan + 1, 1, color);
    }
}
