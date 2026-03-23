#include "touch_ads.h"

static I2C_HandleTypeDef *ads_i2c = NULL;

/* ═══════════════════════════════════════════════════
   Private — GPIO helpers
   Same drive logic as before — only ADC part changes
═══════════════════════════════════════════════════ */
static void tp_output(GPIO_TypeDef *port, uint16_t pin, GPIO_PinState state)
{
    GPIO_InitTypeDef g = {0};
    g.Pin   = pin;
    g.Mode  = GPIO_MODE_OUTPUT_PP;
    g.Pull  = GPIO_NOPULL;
    g.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(port, &g);
    HAL_GPIO_WritePin(port, pin, state);
}

static void tp_input_float(GPIO_TypeDef *port, uint16_t pin)
{
    GPIO_InitTypeDef g = {0};
    g.Pin   = pin;
    g.Mode  = GPIO_MODE_INPUT;
    g.Pull  = GPIO_NOPULL;
    HAL_GPIO_Init(port, &g);
}

static void tp_restore_all(void)
{
    tp_input_float(TP1_GPIO_Port, TP1_Pin);
    tp_input_float(TP2_GPIO_Port, TP2_Pin);
    tp_input_float(TP3_GPIO_Port, TP3_Pin);
    tp_input_float(TP4_GPIO_Port, TP4_Pin);
}

/* ═══════════════════════════════════════════════════
   Private — ADS1115 single conversion
   channel: ADS1115_MUX_AIN0  or  ADS1115_MUX_AIN1
   Returns signed 16-bit raw value
═══════════════════════════════════════════════════ */
static int16_t ads_read(uint16_t channel)
{
    /* build config word */
    uint16_t config = ADS1115_OS_START
                    | channel
                    | ADS1115_PGA_2V
                    | ADS1115_MODE_SINGLE
                    | ADS1115_DR_128SPS
                    | ADS1115_COMP_OFF;

    /* write config register — big endian */
    uint8_t buf[3];
    buf[0] = ADS1115_REG_CONFIG;
    buf[1] = (uint8_t)(config >> 8);
    buf[2] = (uint8_t)(config & 0xFF);
    if (HAL_I2C_Master_Transmit(ads_i2c, ADS1115_ADDR, buf, 3, 100) != HAL_OK)
        return 0;

    /* wait for conversion — 128 SPS = ~8ms per sample */
    HAL_Delay(10);

    /* point to conversion register */
    buf[0] = ADS1115_REG_CONV;
    if (HAL_I2C_Master_Transmit(ads_i2c, ADS1115_ADDR, buf, 1, 100) != HAL_OK)
        return 0;

    /* read 2 bytes result */
    uint8_t res[2] = {0};
    if (HAL_I2C_Master_Receive(ads_i2c, ADS1115_ADDR, res, 2, 100) != HAL_OK)
        return 0;

    /* combine bytes — big endian signed */
    return (int16_t)((res[0] << 8) | res[1]);
}

/* ═══════════════════════════════════════════════════
   Private — map raw to screen pixels
═══════════════════════════════════════════════════ */
static uint16_t tp_map(int16_t raw, int16_t in_min,
                        int16_t in_max, uint16_t out_max)
{
    if (raw <= in_min) return 0;
    if (raw >= in_max) return out_max;
    return (uint16_t)((int32_t)(raw - in_min) * out_max
                      / (in_max - in_min));
}

/* ═══════════════════════════════════════════════════
   TOUCH_ADS_Init
   Call once — pass your I2C handle
   Returns 1 if ADS1115 found, 0 if not
═══════════════════════════════════════════════════ */
uint8_t TOUCH_ADS_Init(I2C_HandleTypeDef *hi2c)
{
    ads_i2c = hi2c;

    /* probe — try to talk to ADS1115 */
    if (HAL_I2C_IsDeviceReady(ads_i2c, ADS1115_ADDR, 2, 100) != HAL_OK)
        return 0;   /* not found */

    return 1;
}

/* ═══════════════════════════════════════════════════
   TOUCH_ADS_IsPressed
   Drive X strip, read A0 (TP4) via ADS1115
   No GPIO pull-up tricks needed — ADS1115 reads
   the actual voltage cleanly
═══════════════════════════════════════════════════ */
uint8_t TOUCH_ADS_IsPressed(void)
{
    /* drive Y strip */
    tp_output(TP1_GPIO_Port, TP1_Pin, GPIO_PIN_SET);    /* Y+ HIGH */
    tp_output(TP2_GPIO_Port, TP2_Pin, GPIO_PIN_RESET);  /* Y- LOW  */
    tp_input_float(TP3_GPIO_Port, TP3_Pin);             /* float   */
    tp_input_float(TP4_GPIO_Port, TP4_Pin);             /* sense   */
    HAL_Delay(3);

    /* read A0 = TP4 */
    int16_t val = ads_read(ADS1115_MUX_AIN0);

    tp_restore_all();

    return (val > TOUCH_PRESS_THRESHOLD) ? 1 : 0;
}

/* ═══════════════════════════════════════════════════
   TOUCH_ADS_Read
   Full X+Y read with stability check
   Returns Touch_t with mapped coordinates

   Reading X:
     Drive Y strip  TP1(Y+)=H  TP2(Y-)=L
     ADS A0 reads TP4 (X- sense) → X position

   Reading Y:
     Drive X strip  TP3(X+)=H  TP4(X-)=L
     ADS A1 reads TP2 (Y- sense) → Y position
═══════════════════════════════════════════════════ */
Touch_t TOUCH_ADS_Read(void)
{
    Touch_t result = {0, 0, 0, 0, 0};
    if (ads_i2c == NULL) return result;

    /* ── Read X (first pass) ─────────────────── */
    tp_output(TP1_GPIO_Port, TP1_Pin, GPIO_PIN_SET);    /* Y+ HIGH */
    tp_output(TP2_GPIO_Port, TP2_Pin, GPIO_PIN_RESET);  /* Y- LOW  */
    tp_input_float(TP3_GPIO_Port, TP3_Pin);
    tp_input_float(TP4_GPIO_Port, TP4_Pin);
    HAL_Delay(3);

    int16_t raw_x1 = ads_read(ADS1115_MUX_AIN0);  /* A0 = TP4 */

    /* save raw and exit early if below threshold */
    result.raw_x = raw_x1;
    if (raw_x1 < TOUCH_PRESS_THRESHOLD) {
        tp_restore_all();
        return result;
    }

    /* ── Read Y ──────────────────────────────── */
    tp_output(TP3_GPIO_Port, TP3_Pin, GPIO_PIN_SET);    /* X+ HIGH */
    tp_output(TP4_GPIO_Port, TP4_Pin, GPIO_PIN_RESET);  /* X- LOW  */
    tp_input_float(TP1_GPIO_Port, TP1_Pin);
    tp_input_float(TP2_GPIO_Port, TP2_Pin);
    HAL_Delay(3);

    int16_t raw_y = ads_read(ADS1115_MUX_AIN1);   /* A1 = TP2 */
    result.raw_y = raw_y;

    /* ── Read X again (stability check) ─────── */
    tp_output(TP1_GPIO_Port, TP1_Pin, GPIO_PIN_SET);
    tp_output(TP2_GPIO_Port, TP2_Pin, GPIO_PIN_RESET);
    tp_input_float(TP3_GPIO_Port, TP3_Pin);
    tp_input_float(TP4_GPIO_Port, TP4_Pin);
    HAL_Delay(3);

    int16_t raw_x2 = ads_read(ADS1115_MUX_AIN0);

    tp_restore_all();

    result.raw_x = raw_x2;

    /* reject if two X reads differ too much = panel disturbed */
    int16_t diff = (raw_x1 > raw_x2) ? raw_x1 - raw_x2
                                      : raw_x2 - raw_x1;
    if (diff > 500) return result;

    /* reject if second X dropped below threshold */
    if (raw_x2 < TOUCH_PRESS_THRESHOLD) return result;

    /* map to screen pixels */
    result.x       = tp_map(raw_x2, TOUCH_RAW_X_MIN, TOUCH_RAW_X_MAX,
                             TOUCH_SCR_W - 1);
    result.y       = tp_map(raw_y,  TOUCH_RAW_Y_MIN, TOUCH_RAW_Y_MAX,
                             TOUCH_SCR_H - 1);
    result.pressed = 1;

    return result;
}
