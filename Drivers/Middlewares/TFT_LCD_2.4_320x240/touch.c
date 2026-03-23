#include "touch.h"

static ADC_HandleTypeDef *touch_adc = NULL;

/* ═══════════════════════════════════════════════════
   TOUCH_Init
   Call once — pass &hadc1 from main
═══════════════════════════════════════════════════ */
void TOUCH_Init(ADC_HandleTypeDef *hadc)
{
    touch_adc = hadc;
}

/* ═══════════════════════════════════════════════════
   Private — set a TP pin as output and drive it
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

/* ═══════════════════════════════════════════════════
   Private — set a TP pin as floating input
═══════════════════════════════════════════════════ */
static void tp_float(GPIO_TypeDef *port, uint16_t pin)
{
    GPIO_InitTypeDef g = {0};
    g.Pin   = pin;
    g.Mode  = GPIO_MODE_INPUT;
    g.Pull  = GPIO_NOPULL;
    HAL_GPIO_Init(port, &g);
}

/* ═══════════════════════════════════════════════════
   Private — set a TP pin as analog (for ADC read)
═══════════════════════════════════════════════════ */
static void tp_analog(GPIO_TypeDef *port, uint16_t pin)
{
    GPIO_InitTypeDef g = {0};
    g.Pin   = pin;
    g.Mode  = GPIO_MODE_ANALOG;
    g.Pull  = GPIO_NOPULL;
    HAL_GPIO_Init(port, &g);
}

/* ═══════════════════════════════════════════════════
   Private — restore all 4 TP pins to GPIO_Input
   Call after every read sequence
═══════════════════════════════════════════════════ */
static void tp_restore_all(void)
{
    GPIO_InitTypeDef g = {0};
    g.Mode  = GPIO_MODE_INPUT;
    g.Pull  = GPIO_NOPULL;
    g.Speed = GPIO_SPEED_FREQ_LOW;
    g.Pin = TP1_Pin; HAL_GPIO_Init(TP1_GPIO_Port, &g);
    g.Pin = TP2_Pin; HAL_GPIO_Init(TP2_GPIO_Port, &g);
    g.Pin = TP3_Pin; HAL_GPIO_Init(TP3_GPIO_Port, &g);
    g.Pin = TP4_Pin; HAL_GPIO_Init(TP4_GPIO_Port, &g);
}

/* ═══════════════════════════════════════════════════
   Private — read ADC channel, average TOUCH_SAMPLES
═══════════════════════════════════════════════════ */
static uint16_t adc_read(uint32_t channel)
{
    ADC_ChannelConfTypeDef cfg = {0};
    cfg.Channel      = channel;
    cfg.Rank         = 1;
    cfg.SamplingTime = ADC_SAMPLETIME_112CYCLES;
    HAL_ADC_ConfigChannel(touch_adc, &cfg);

    uint32_t sum = 0;
    for (uint8_t i = 0; i < TOUCH_SAMPLES; i++) {
        HAL_ADC_Start(touch_adc);
        HAL_ADC_PollForConversion(touch_adc, 10);
        sum += HAL_ADC_GetValue(touch_adc);
        HAL_ADC_Stop(touch_adc);
    }
    return (uint16_t)(sum / TOUCH_SAMPLES);
}

/* ═══════════════════════════════════════════════════
   Private — map raw ADC to screen pixels
═══════════════════════════════════════════════════ */
static uint16_t tp_map(uint16_t raw, uint16_t in_min,
                        uint16_t in_max, uint16_t out_max)
{
    if (raw <= in_min) return 0;
    if (raw >= in_max) return out_max;
    return (uint16_t)((uint32_t)(raw - in_min) * out_max
                      / (in_max - in_min));
}

/* ═══════════════════════════════════════════════════
   TOUCH_IsPressed
   Fast GPIO check — no ADC needed

   Drive TP3(X+)=HIGH, TP4(X-)=LOW
   Set TP2(Y-) as input pull-up
   If TP2 reads LOW → panel layers touching → pressed
═══════════════════════════════════════════════════ */
uint8_t TOUCH_IsPressed(void)
{
    /* Drive Y strip: TP1(Y+)=H, TP2(Y-)=L
       Read X sense:  TP4(X-) via ADC
       No touch → TP4 reads ~4  (pulled to GND by TP2)
       Touch    → TP4 reads >TOUCH_PRESS_THRESHOLD      */
    tp_output(TP1_GPIO_Port, TP1_Pin, GPIO_PIN_SET);
    tp_output(TP2_GPIO_Port, TP2_Pin, GPIO_PIN_RESET);
    tp_analog(TP3_GPIO_Port, TP3_Pin);
    tp_analog(TP4_GPIO_Port, TP4_Pin);
    HAL_Delay(2);

    uint16_t val = adc_read(ADC_CHANNEL_14);  /* TP4 = PC4 = IN14 */

    tp_restore_all();

    return (val > TOUCH_PRESS_THRESHOLD) ? 1 : 0;
}

/* ═══════════════════════════════════════════════════
   TOUCH_Read
   Returns mapped x,y coordinates and pressed flag

   Read X:  TP1(Y+)=H  TP2(Y-)=L  TP3,TP4 analog
            read TP4 (X-) ADC_CHANNEL_14
            high raw = right side

   Read Y:  TP3(X+)=H  TP4(X-)=L  TP1,TP2 analog
            read TP2 (Y-) ADC_CHANNEL_12
            high raw = bottom
═══════════════════════════════════════════════════ */
Touch_t TOUCH_Read(void)
{
    Touch_t result = {0, 0, 0, 0, 0};
    if (touch_adc == NULL) return result;

    /* ── Read X (first time) ─────────────────── */
    tp_output(TP1_GPIO_Port, TP1_Pin, GPIO_PIN_SET);
    tp_output(TP2_GPIO_Port, TP2_Pin, GPIO_PIN_RESET);
    tp_analog(TP3_GPIO_Port, TP3_Pin);
    tp_analog(TP4_GPIO_Port, TP4_Pin);
    HAL_Delay(5);
    uint16_t raw_x1 = adc_read(ADC_CHANNEL_14);

    /* below threshold — definitely not pressed */
    if (raw_x1 < TOUCH_PRESS_THRESHOLD) {
        result.raw_x = raw_x1;
        tp_restore_all();
        return result;
    }

    /* ── Read Y ──────────────────────────────── */
    tp_output(TP3_GPIO_Port, TP3_Pin, GPIO_PIN_SET);
    tp_output(TP4_GPIO_Port, TP4_Pin, GPIO_PIN_RESET);
    tp_analog(TP1_GPIO_Port, TP1_Pin);
    tp_analog(TP2_GPIO_Port, TP2_Pin);
    HAL_Delay(5);
    uint16_t raw_y = adc_read(ADC_CHANNEL_12);

    /* ── Read X again (stability check) ─────── */
    tp_output(TP1_GPIO_Port, TP1_Pin, GPIO_PIN_SET);
    tp_output(TP2_GPIO_Port, TP2_Pin, GPIO_PIN_RESET);
    tp_analog(TP3_GPIO_Port, TP3_Pin);
    tp_analog(TP4_GPIO_Port, TP4_Pin);
    HAL_Delay(5);
    uint16_t raw_x2 = adc_read(ADC_CHANNEL_14);

    tp_restore_all();

    result.raw_x = raw_x2;
    result.raw_y = raw_y;

    /* reject if the two X readings differ too much
       large difference = panel disturbed = bad read  */
    uint16_t diff = (raw_x1 > raw_x2) ? raw_x1 - raw_x2
                                       : raw_x2 - raw_x1;
    if (diff > 200) return result;

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
