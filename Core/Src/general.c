#include "general.h"
#include <stdio.h>
#include <string.h>

/* ═══════════════════════════════════════════════════
   Notification content — edit text here
═══════════════════════════════════════════════════ */
static const char *notif_app[3]      = { "WhatsApp",            "Gmail",               "System"           };
static const char *notif_msg[3]      = { "Hey, are you there?", "New message received", "Update available" };
static const char *notif_time[3]     = { "now",                 "5m",                  "1h"               };
static const uint16_t notif_color[3] = { 0x07E0, 0x3D7F, 0xFD20 };
/*                                       Green   Blue    Orange  */

/* ═══════════════════════════════════════════════════
   Theme colors — dark blue Android
   Standard colors (Green, Red, Yellow, Orange, White)
   come from tft.h — no duplicates here
═══════════════════════════════════════════════════ */
#define C_BG        0xFFFF   /* #FFFFFF — white background       */
#define C_STATUSBAR 0xEF7D   /* #F0F0F0 — light gray status bar  */
#define C_CARD      0xF7BE   /* #F5F5F5 — card background        */
#define C_PANEL     0xEF7D   /* #F0F0F0 — panel background       */
#define C_QS_ON     0xC61F   /* #C8E0FF — blue tinted ON tile    */
#define C_QS_OFF    0xE71C   /* #E8E8E8 — light gray OFF tile    */
#define C_ACCENT    0x034F   /* #0066FF — Android blue (kept)    */
#define C_SEP       0xC618   /* #C0C0C0 — light separator        */
#define C_TEXT      0x0000   /* black   — primary text           */
#define C_TEXT2     0x4208   /* #404040 — dark gray secondary    */
#define C_TEXT3     0x8410   /* #808080 — mid gray hint text     */

/* ═══════════════════════════════════════════════════
   Layout  (240 × 320 portrait)
═══════════════════════════════════════════════════ */
#define SCR_W   240
#define SCR_H   320
#define SB_H     20

#define TIME_Y   (SB_H + 6)
#define SBAR_Y   (TIME_Y + 30)
#define DATE_Y   (SBAR_Y + 6)
#define SEP1_Y   (DATE_Y + 14)
#define WTH_Y    (SEP1_Y + 6)
#define LOC_Y    (WTH_Y + 14)
#define SEP2_Y   (LOC_Y + 14)
#define NH_Y     (SEP2_Y + 6)
#define N1_Y     (NH_Y + 12)
#define N2_Y     (N1_Y + 42)
#define N3_Y     (N2_Y + 42)
#define FOOT_Y   (SCR_H - 16)

#define NC_W    232
#define NC_H     38
#define NC_X      4

#define NP_H    255
#define QS_Y    (SB_H + 4)
#define QS_H     32
#define NPS_Y   (QS_Y + QS_H + 8)
#define NPN1_Y  (NPS_Y + 4)
#define NPN2_Y  (NPN1_Y + 46)
#define NPN3_Y  (NPN2_Y + 46)

/* ═══════════════════════════════════════════════════
   State
═══════════════════════════════════════════════════ */
static uint8_t  notif_open  = 0;
static uint32_t boot_tick   = 0;
static uint8_t  prev_sec    = 0xFF;
static uint8_t  prev_min    = 0xFF;
static uint8_t  need_redraw = 1;
static uint8_t  btn_prev    = 1;    /* 1 = released */

/* ═══════════════════════════════════════════════════
   Print macros — each uses the correct bg color
   so text blends into background instead of showing
   a black rectangle behind every character
═══════════════════════════════════════════════════ */
#define PRINT_BG(x,y,c,s)    TFT_PrintFont(x, y, c, C_BG,      &Font_7x10, s)
#define PRINT_SB(x,y,c,s)    TFT_PrintFont(x, y, c, C_STATUSBAR,&Font_7x10, s)
#define PRINT_CD(x,y,c,s)    TFT_PrintFont(x, y, c, C_CARD,    &Font_7x10, s)
#define PRINT_PN(x,y,c,s)    TFT_PrintFont(x, y, c, C_PANEL,   &Font_7x10, s)
#define PRINT_QS(x,y,c,bg,s) TFT_PrintFont(x, y, c, bg,        &Font_7x10, s)

/* ═══════════════════════════════════════════════════
   Time helper
═══════════════════════════════════════════════════ */
static void get_time(uint8_t *h, uint8_t *m, uint8_t *s)
{
    uint32_t elapsed = (HAL_GetTick() - boot_tick) / 1000;
    uint32_t total   = (uint32_t)(PHONE_START_HOUR * 3600
                     + PHONE_START_MIN  * 60
                     + PHONE_START_SEC)
                     + elapsed;
    *h = (uint8_t)((total / 3600) % 24);
    *m = (uint8_t)((total / 60)   % 60);
    *s = (uint8_t)(total           % 60);
}

/* ═══════════════════════════════════════════════════
   Icon helpers
═══════════════════════════════════════════════════ */
static void draw_signal(uint16_t x, uint16_t y, uint8_t level)
{
    for (uint8_t i = 0; i < 4; i++) {
        uint16_t bh = 3 + i * 3;
        uint16_t bx = x + i * 5;
        uint16_t by = y + 12 - bh;
        TFT_FillRect(bx, y,  4, 12, C_STATUSBAR);   /* clear column first */
        TFT_FillRect(bx, by, 4, bh, (i < level) ? Green : C_SEP);
    }
}

static void draw_wifi(uint16_t x, uint16_t y, uint8_t on)
{
    uint16_t c = on ? C_ACCENT : C_SEP;
    TFT_FillRect(x, y, 16, 16, C_STATUSBAR);         /* clear area first  */
    TFT_FillRect(x + 5, y,     6,  2, c);
    TFT_FillRect(x + 3, y + 4, 10, 2, c);
    TFT_FillRect(x + 1, y + 8, 14, 2, c);
    TFT_FillCircle(x + 7, y + 13, 2, on ? C_ACCENT : C_SEP);
}

static void draw_battery(uint16_t x, uint16_t y, uint8_t pct, uint8_t charging)
{
    TFT_FillRect(x, y, 28, 12, C_STATUSBAR);          /* clear area first  */
    TFT_DrawRect(x, y, 24, 12, C_TEXT2);
    TFT_FillRect(x + 24, y + 3, 3, 6, C_TEXT2);
    uint16_t fw = (uint16_t)((float)pct / 100.0f * 22.0f);
    uint16_t fc = pct > 50 ? Green : pct > 20 ? Yellow : Red;
    if (fw > 0) TFT_FillRect(x + 1, y + 1, fw, 10, fc);
    if (charging) {
        TFT_DrawLine(x + 9,  y + 2,  x + 6,  y + 6,  Yellow);
        TFT_DrawLine(x + 6,  y + 6,  x + 11, y + 6,  Yellow);
        TFT_DrawLine(x + 11, y + 6,  x + 8,  y + 10, Yellow);
    }
}

/* ═══════════════════════════════════════════════════
   Status bar — bg passed in so same func used for
   both main screen and panel
═══════════════════════════════════════════════════ */
static void draw_statusbar(uint16_t bg)
{
    TFT_FillRect(0, 0, SCR_W, SB_H, bg);
    draw_signal(6, 4, PHONE_SIGNAL);
    draw_wifi(30, 2, PHONE_WIFI_ON);

    uint8_t h, m, s;
    get_time(&h, &m, &s);
    char buf[8];
    sprintf(buf, "%02d:%02d", h, m);
    uint16_t tw = (uint16_t)(strlen(buf) * 7);
    TFT_PrintFont((SCR_W - tw) / 2, 5, C_TEXT, bg, &Font_7x10, buf);

    draw_battery(SCR_W - 32, 4, PHONE_BATTERY_PCT, PHONE_CHARGING);
}

/* ═══════════════════════════════════════════════════
   Main screen — full draw
═══════════════════════════════════════════════════ */
static void draw_main_screen(void)
{
    TFT_Clear(C_BG);
    draw_statusbar(C_STATUSBAR);

    uint8_t h, m, s;
    get_time(&h, &m, &s);

    /* big time */
    char tbuf[6];
    sprintf(tbuf, "%02d:%02d", h, m);
    uint16_t tlen = (uint16_t)(strlen(tbuf) * 16);
    TFT_PrintFont((SCR_W - tlen) / 2, TIME_Y, C_TEXT, C_BG, &Font_16x26, tbuf);

    /* seconds progress bar */
    TFT_FillRect(20, SBAR_Y, 200, 3, C_SEP);
    uint16_t sbw = (uint16_t)((float)s / 59.0f * 200.0f);
    TFT_FillRect(20, SBAR_Y, sbw, 3, C_ACCENT);

    /* date */
    uint16_t dlen = (uint16_t)(strlen(PHONE_DATE) * 7);
    PRINT_BG((SCR_W - dlen) / 2, DATE_Y, C_TEXT2, PHONE_DATE);

    /* separator */
    TFT_FillRect(20, SEP1_Y, 200, 1, C_SEP);

    /* weather row */
    PRINT_BG(10,  WTH_Y, C_ACCENT, PHONE_WEATHER_TEMP);
    PRINT_BG(52,  WTH_Y, C_TEXT2,  PHONE_WEATHER_DESC);
    uint16_t clen = (uint16_t)(strlen(PHONE_CITY) * 7);
    PRINT_BG(SCR_W - clen - 6, WTH_Y, C_TEXT3, PHONE_CITY);

    /* location */
    PRINT_BG(10,  LOC_Y, C_TEXT3, "Lat:");
    PRINT_BG(38,  LOC_Y, C_TEXT2, PHONE_LAT);
    PRINT_BG(124, LOC_Y, C_TEXT3, "Lon:");
    PRINT_BG(152, LOC_Y, C_TEXT2, PHONE_LON);

    /* separator */
    TFT_FillRect(20, SEP2_Y, 200, 1, C_SEP);

    /* notifications label */
    PRINT_BG(10, NH_Y, C_TEXT3, "NOTIFICATIONS");

    /* notification cards */
    for (uint8_t i = 0; i < NOTIF_COUNT && i < 3; i++) {
        uint16_t cy = (i == 0) ? N1_Y : (i == 1) ? N2_Y : N3_Y;
        TFT_FillRect(NC_X, cy, NC_W, NC_H, C_CARD);
        TFT_FillRect(NC_X, cy, 4,    NC_H, notif_color[i]);
        TFT_DrawRect(NC_X, cy, NC_W, NC_H, C_SEP);
        PRINT_CD(NC_X + 8, cy + 5,  notif_color[i], notif_app[i]);
        uint16_t ntlen = (uint16_t)(strlen(notif_time[i]) * 7);
        PRINT_CD(NC_X + NC_W - ntlen - 4, cy + 5,  C_TEXT3, notif_time[i]);
        PRINT_CD(NC_X + 8, cy + 20, C_TEXT2, notif_msg[i]);
    }

    /* footer hint */
    PRINT_BG(34, FOOT_Y, C_TEXT3, "v press button for panel v");

    prev_min = m;
    prev_sec = s;
}

/* ═══════════════════════════════════════════════════
   Main screen — partial update only
═══════════════════════════════════════════════════ */
static void update_main_screen(void)
{
    uint8_t h, m, s;
    get_time(&h, &m, &s);

    /* seconds bar — every second */
    if (s != prev_sec) {
        TFT_FillRect(20, SBAR_Y, 200, 3, C_SEP);
        uint16_t sbw = (uint16_t)((float)s / 59.0f * 200.0f);
        TFT_FillRect(20, SBAR_Y, sbw, 3, C_ACCENT);
        prev_sec = s;

        /* small time in status bar */
        TFT_FillRect((SCR_W - 42) / 2, 4, 42, 12, C_STATUSBAR);
        char buf[8];
        sprintf(buf, "%02d:%02d", h, m);
        TFT_PrintFont((SCR_W - (uint16_t)(strlen(buf) * 7)) / 2,
                      5, C_TEXT, C_STATUSBAR, &Font_7x10, buf);
    }

    /* big time — every minute */
    if (m != prev_min) {
        char tbuf[6];
        sprintf(tbuf, "%02d:%02d", h, m);
        uint16_t tlen = (uint16_t)(strlen(tbuf) * 16);
        TFT_FillRect(0, TIME_Y, SCR_W, 28, C_BG);
        TFT_PrintFont((SCR_W - tlen) / 2, TIME_Y,
        		C_TEXT, C_BG, &Font_16x26, tbuf);
        prev_min = m;
    }
}

/* ═══════════════════════════════════════════════════
   Notification panel — full draw
═══════════════════════════════════════════════════ */
static void draw_notif_panel(void)
{
    TFT_FillRect(0, 0, SCR_W, NP_H, C_PANEL);
    draw_statusbar(C_STATUSBAR);

    /* panel title row */
    TFT_FillRect(0, SB_H, SCR_W, 18, C_PANEL);
    PRINT_PN(10,         SB_H + 4, C_TEXT,   "Notifications");
    PRINT_PN(SCR_W - 50, SB_H + 4, C_TEXT2, "X close");

    /* quick settings — 4 tiles */
    uint16_t   qx[4]  = { 4,  64, 124, 184 };
    const char *ql[4] = { "WiFi", "BT", "Loc", "Plane" };
    uint8_t    qon[4] = { PHONE_WIFI_ON, 0, 1, 0 };
    uint16_t   qtc[4] = { C_ACCENT, C_TEXT3, C_ACCENT, C_TEXT3 };

    for (uint8_t i = 0; i < 4; i++) {
        uint16_t bg = qon[i] ? C_QS_ON  : C_QS_OFF;
        uint16_t tc = qon[i] ? qtc[i]   : C_TEXT3;
        TFT_FillRect(qx[i], QS_Y, 56, QS_H, bg);
        TFT_DrawRect(qx[i], QS_Y, 56, QS_H, C_SEP);
        uint16_t llen = (uint16_t)(strlen(ql[i]) * 7);
        PRINT_QS(qx[i] + (56 - llen) / 2,
                 QS_Y + (QS_H - 10) / 2, tc, bg, ql[i]);
    }

    /* separator */
    TFT_FillRect(10, NPS_Y, SCR_W - 20, 1, C_SEP);

    /* notification cards in panel */
    uint16_t pnc_w = SCR_W - 8;
    for (uint8_t i = 0; i < NOTIF_COUNT && i < 3; i++) {
        uint16_t cy = (i == 0) ? NPN1_Y : (i == 1) ? NPN2_Y : NPN3_Y;
        TFT_FillRect(4, cy, pnc_w, 40, C_CARD);
        TFT_FillRect(4, cy, 4,     40, notif_color[i]);
        TFT_DrawRect(4, cy, pnc_w, 40, C_SEP);
        PRINT_CD(14, cy + 5,  notif_color[i], notif_app[i]);
        uint16_t ntl = (uint16_t)(strlen(notif_time[i]) * 7);
        PRINT_CD(pnc_w - ntl - 2, cy + 5,  C_TEXT3, notif_time[i]);
        PRINT_CD(14, cy + 22, C_TEXT2, notif_msg[i]);
    }

    /* close hint */
    PRINT_PN(34, NP_H - 14, C_TEXT3, "^ press button to close ^");

    /* accent strip at bottom edge of panel */
    TFT_FillRect(0, NP_H, SCR_W, 2, C_ACCENT);
}

/* ═══════════════════════════════════════════════════
   Public API
═══════════════════════════════════════════════════ */
void GENERAL_Init(void)
{
    TFT_Init();
    boot_tick = HAL_GetTick();

    TFT_Clear(C_BG);
    TFT_PrintFont(20, 130, C_ACCENT, C_BG, &Font_11x18, "Phone UI Demo");
    PRINT_BG(50, 158, C_TEXT2, "STM32F407 + ILI9341");
    HAL_Delay(800);

    notif_open  = 0;
    need_redraw = 1;
}

void GENERAL_RUN(void)
{

    /* ── display logic ─────────────────────────── */
    if (notif_open) {
        if (need_redraw) {
            draw_notif_panel();
            need_redraw = 0;
        }
        uint8_t h, m, s;
        get_time(&h, &m, &s);
        if (s != prev_sec) {
            TFT_FillRect((SCR_W - 42) / 2, 4, 42, 12, C_STATUSBAR);
            char buf[8];
            sprintf(buf, "%02d:%02d", h, m);
            TFT_PrintFont((SCR_W - (uint16_t)(strlen(buf) * 7)) / 2,
                          5, C_TEXT, C_STATUSBAR, &Font_7x10, buf);
            prev_sec = s;
        }
    } else {
        if (need_redraw) {
            draw_main_screen();
            need_redraw = 0;
        } else {
            update_main_screen();
        }
    }
}

void GENERAL_ButtonPress(void)
{
    notif_open  = !notif_open;
    need_redraw = 1;
}
