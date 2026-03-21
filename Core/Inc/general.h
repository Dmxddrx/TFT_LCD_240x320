#ifndef GENERAL_H
#define GENERAL_H

#include "main.h"
#include "tft.h"

/* ═══════════════════════════════════════════════════
   Manual data — edit these to change what shows
═══════════════════════════════════════════════════ */

/* Fake running clock — starts at this time on boot */
#define PHONE_START_HOUR   00
#define PHONE_START_MIN    35
#define PHONE_START_SEC    00

/* Date string */
#define PHONE_DATE         "Saturday, 22 Mar 2026"

/* Weather */
#define PHONE_WEATHER_TEMP "28 C"
#define PHONE_WEATHER_DESC "Partly Cloudy"
#define PHONE_CITY         "Katubedda, LK"

/* Location */
#define PHONE_LAT          "7.2106 N"
#define PHONE_LON          "79.8367 E"

/* Status bar */
#define PHONE_SIGNAL       4     /* 0-4 bars          */
#define PHONE_WIFI_ON      1     /* 1=connected 0=off */
#define PHONE_BATTERY_PCT  60    /* 0-100             */
#define PHONE_CHARGING     1     /* 1=show bolt       */

/* Number of notifications shown (max 3) */
#define NOTIF_COUNT        3

/* ═══════════════════════════════════════════════════
   Public API
═══════════════════════════════════════════════════ */
void GENERAL_Init(void);
void GENERAL_RUN(void);
void GENERAL_ButtonPress(void);  /* call when physical button pressed */

#endif /* GENERAL_H */
