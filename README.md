# TFT LCD 240x320 Interface via FSMC

![MCU](https://img.shields.io/badge/MCU-STM32F407VET6-blue.svg)
![Framework](https://img.shields.io/badge/Framework-STM32CubeIDE-green.svg)
![Interface](https://img.shields.io/badge/Interface-FSMC_8--bit-orange.svg)

## Overview
This repository contains the firmware and configuration files for interfacing a 240x320 TFT LCD display with the **STM32F407VET6** microcontroller (often referred to as the "Black Board"). 

Instead of relying on standard SPI or GPIO bit-banging, this project leverages the STM32's **Flexible Static Memory Controller (FSMC)**. By configuring the FSMC, the Cortex-M4 core treats the display as an external memory block. This hardware acceleration drastically increases data transfer speeds and refresh rates, leaving the CPU free to handle other demanding tasks.

## Hardware Setup
* **Microcontroller:** STM32F407VETx (100-pin LQFP)
* **Core Clock:** 168 MHz (HSE + Main PLL)
* **Display Interface:** 8080/6800 Parallel Interface

### FSMC Configuration & Pin Mapping
The display is mapped to **Bank 1 (NOR/PSRAM 1)** with an 8-bit data width. 

| LCD Signal | FSMC Signal | STM32 Pin | Description |
| :--- | :--- | :--- | :--- |
| **RS / DC** | `FSMC_A18` | **PD13** | Register Select. Switches between Command (`RS=0`) and Data (`RS=1`). |
| **WR** | `FSMC_NWE` | **PD5** | Write Enable (Active Low). Pulses to latch data into the LCD. |
| **RD** | `FSMC_NOE` | **PD4** | Output Enable (Active Low). Pulses to read data from the LCD. |
| **CS** | `FSMC_NE1` | **PD7** | Chip Enable (Active Low). Activates the display on the bus. |
| **D0 - D7** | `FSMC_D[0:7]` | Various | 8-bit Parallel Data Highway. |
| **RST** | `GPIO_Output` | **PB12** | Hardware Reset for the LCD. |
| **BLK** | `GPIO_Output` | **PB1** | Backlight Control. |

*Note: Address line **A18** was chosen because pin **PD13** functions as the Register Select (LCD_RS) line in this specific hardware configuration.*

### Critical Timing Parameters
To ensure signal integrity between the 168 MHz MCU and the display controller, the FSMC timings (in HCLK cycles) are configured as follows:
* **Address Setup Time:** `5`
* **Data Setup Time:** `9`
* **Bus Turnaround Time:** `0`

## Repository Structure
* `/.settings/`, `/Debug/`: IDE-specific build and configuration folders.
* `/Core/`: Contains the main application logic, initialization code, and interrupts.
* `/Drivers/`: STM32F4xx HAL Driver libraries and CMSIS components.
* `/Datasheets/`: Reference documentation for the hardware.
* `TFT_Screen.ioc`: The STM32CubeMX configuration file used to generate the initialization code.
* `TFT_Screen.pdf`: A detailed hardware configuration and power consumption report generated via CubeMX.

## Getting Started
1. Clone this repository to your local machine:
   ```bash
   git clone [https://github.com/Dmxddrx/TFT_LCD_240x320.git](https://github.com/Dmxddrx/TFT_LCD_240x320.git)
