#ifndef TOUCH_H
#define TOUCH_H

#include "main.h"
#include <stdint.h>

/* ═══════════════════════════════════════════════════
   4-wire resistive touch — verified pin assignments
   Confirmed by ADC measurement on your hardware

   TP1 (PC1) = Y+   drive HIGH when reading X axis
   TP2 (PC2) = Y-   drive LOW  when reading X axis
                     ADC sense  when reading Y axis
   TP3 (PC3) = X+   drive HIGH when reading Y axis
   TP4 (PC4) = X-   drive LOW  when reading Y axis
                     ADC sense  when reading X axis

   Reading X:  TP1=H  TP2=L  float TP3,TP4  read TP4 ADC
   Reading Y:  TP3=H  TP4=L  float TP1,TP2  read TP2 ADC
   Press det:  TP3=H  TP4=L  TP2 pull-up    read TP2 GPIO
═══════════════════════════════════════════════════ */

/* ── calibration — adjust after testing ──────────
   Measure: press left edge → note TP4 raw value
            press right edge→ note TP4 raw value
            press top edge  → note TP2 raw value
            press bot edge  → note TP2 raw value      */
#define TOUCH_RAW_X_MIN    1800   /* TP4 raw at left edge    */
#define TOUCH_RAW_X_MAX    3100   /* TP4 raw at right edge   */
#define TOUCH_RAW_Y_MIN     100   /* TP2 raw at top edge     */
#define TOUCH_RAW_Y_MAX    2000   /* TP2 raw at bottom edge  */

/* ── threshold — below this = no press ──────────── */
#define TOUCH_PRESS_THRESHOLD  200   /* TP4 no-touch = ~4  */

/* ── samples averaged per read ──────────────────── */
#define TOUCH_SAMPLES     12

/* ── screen size for coordinate mapping ─────────── */
#define TOUCH_SCR_W      240
#define TOUCH_SCR_H      320

/* ═══════════════════════════════════════════════════
   Result struct
═══════════════════════════════════════════════════ */
typedef struct {
    uint16_t x;        /* mapped screen pixel X    */
    uint16_t y;        /* mapped screen pixel Y    */
    uint16_t raw_x;    /* raw ADC from TP4         */
    uint16_t raw_y;    /* raw ADC from TP2         */
    uint8_t  pressed;  /* 1=touching 0=none        */
} Touch_t;

/* ═══════════════════════════════════════════════════
   API
═══════════════════════════════════════════════════ */
void    TOUCH_Init(ADC_HandleTypeDef *hadc);
Touch_t TOUCH_Read(void);
uint8_t TOUCH_IsPressed(void);

#endif /* TOUCH_H */
