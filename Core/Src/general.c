#include "general.h"

/* ═══════════════════════════════════════════════════
   GENERAL_Init
   Call once in main  USER CODE BEGIN 2
═══════════════════════════════════════════════════ */
void GENERAL_Init(void)
{
    TFT_Init();

#if ENABLE_TFT_SELFTEST
    GENERAL_TFT_SelfTest();
#endif

    TFT_Clear(Black);
    TFT_Print(0, 0, White, "Ready");
}

/* ═══════════════════════════════════════════════════
   GENERAL_RUN
   Called every iteration of while(1)
   Put your main application logic here
═══════════════════════════════════════════════════ */
void GENERAL_RUN(void)
{
    /* add your repeating logic here */
}

/* ═══════════════════════════════════════════════════
   GENERAL_TFT_SelfTest
   Red → green → blue — confirms display working
═══════════════════════════════════════════════════ */
#if ENABLE_TFT_SELFTEST
void GENERAL_TFT_SelfTest(void)
{
    TFT_Clear(Red);
    HAL_Delay(600);
    TFT_Clear(Green);
    HAL_Delay(600);
    TFT_Clear(Blue);
    HAL_Delay(600);
}
#endif
