#ifndef TOUCH_ADS_H
#define TOUCH_ADS_H

#include "main.h"
#include <stdint.h>

/* ═══════════════════════════════════════════════════
   ADS1115 — 16-bit I2C ADC for 4-wire resistive touch

   Wiring:
     ADS1115 ADDR → GND    I2C address 0x48
     ADS1115 A0   → TP4 (PC4)   X sense wire
     ADS1115 A1   → TP2 (PC2)   Y sense wire
     ADS1115 A2   → not used
     ADS1115 A3   → not used
     ADS1115 SDA  → STM32 SDA
     ADS1115 SCL  → STM32 SCL
     ADS1115 VDD  → 3.3V
     ADS1115 GND  → GND

   GPIO drive (same as before):
     TP1 (PC1) = Y+   drive HIGH when reading X
     TP2 (PC2) = Y-   drive LOW  when reading X
     TP3 (PC3) = X+   drive HIGH when reading Y
     TP4 (PC4) = X-   drive LOW  when reading Y
═══════════════════════════════════════════════════ */

/* ── I2C address (ADDR pin to GND = 0x48) ──────── */
#define ADS1115_ADDR         (0x48 << 1)

/* ── ADS1115 register addresses ─────────────────── */
#define ADS1115_REG_CONV     0x00   /* conversion result   */
#define ADS1115_REG_CONFIG   0x01   /* configuration       */

/* ── Config register bit fields ─────────────────── */
/* OS — start single conversion (write 1) */
#define ADS1115_OS_START     0x8000

/* MUX — single ended channel select */
#define ADS1115_MUX_AIN0     0x4000   /* A0 vs GND — X sense (TP4) */
#define ADS1115_MUX_AIN1     0x5000   /* A1 vs GND — Y sense (TP2) */

/* PGA — programmable gain ±2.048V (safe for 3.3V input) */
#define ADS1115_PGA_2V       0x0400

/* MODE — single shot */
#define ADS1115_MODE_SINGLE  0x0100

/* DR — data rate 128 SPS (~8ms per conversion) */
#define ADS1115_DR_128SPS    0x0080

/* COMP — comparator disabled */
#define ADS1115_COMP_OFF     0x0003

/* ── calibration — raw ADS1115 values at screen edges
   ADS1115 is 16-bit signed: range 0 to 32767 for 0–2.048V
   Adjust after testing on your panel                     */
#define TOUCH_RAW_X_MIN      3000    /* A0 raw at left edge   */
#define TOUCH_RAW_X_MAX      18000   /* A0 raw at right edge  */
#define TOUCH_RAW_Y_MIN      1000    /* A1 raw at top edge    */
#define TOUCH_RAW_Y_MAX      12000   /* A1 raw at bottom edge */

/* ── press threshold — below this = no touch ─────── */
#define TOUCH_PRESS_THRESHOLD  2000

/* ── screen dimensions for coordinate mapping ─────── */
#define TOUCH_SCR_W          240
#define TOUCH_SCR_H          320

/* ── result struct ───────────────────────────────── */
typedef struct {
    uint16_t x;        /* mapped screen X  0 to SCR_W-1 */
    uint16_t y;        /* mapped screen Y  0 to SCR_H-1 */
    int16_t  raw_x;    /* raw ADS1115 A0 value           */
    int16_t  raw_y;    /* raw ADS1115 A1 value           */
    uint8_t  pressed;  /* 1 = touching, 0 = none         */
} Touch_t;

/* ═══════════════════════════════════════════════════
   API
═══════════════════════════════════════════════════ */
uint8_t TOUCH_ADS_Init(I2C_HandleTypeDef *hi2c);
Touch_t TOUCH_ADS_Read(void);
uint8_t TOUCH_ADS_IsPressed(void);

#endif /* TOUCH_ADS_H */
