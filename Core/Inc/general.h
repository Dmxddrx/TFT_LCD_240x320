#ifndef GENERAL_H
#define GENERAL_H

#include "main.h"
#include "tft.h"

/* ═══════════════════════════════════════════════════
   Debug flags — set 1 to enable, 0 to disable
═══════════════════════════════════════════════════ */
#define ENABLE_TFT_SELFTEST    1   /* color test on startup */

/* ═══════════════════════════════════════════════════
   Called once in main  USER CODE BEGIN 2
═══════════════════════════════════════════════════ */
void GENERAL_Init(void);

/* ═══════════════════════════════════════════════════
   Called every iteration of while(1)
═══════════════════════════════════════════════════ */
void GENERAL_RUN(void);

/* ═══════════════════════════════════════════════════
   Self-test — enabled by flag above
═══════════════════════════════════════════════════ */
#if ENABLE_TFT_SELFTEST
void GENERAL_TFT_SelfTest(void);
#endif

#endif /* GENERAL_H */
