################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Middlewares/TFT_LCD_2.4_320x240/ILI9341.c \
../Drivers/Middlewares/TFT_LCD_2.4_320x240/fonts.c \
../Drivers/Middlewares/TFT_LCD_2.4_320x240/touch.c \
../Drivers/Middlewares/TFT_LCD_2.4_320x240/touch_ads.c 

OBJS += \
./Drivers/Middlewares/TFT_LCD_2.4_320x240/ILI9341.o \
./Drivers/Middlewares/TFT_LCD_2.4_320x240/fonts.o \
./Drivers/Middlewares/TFT_LCD_2.4_320x240/touch.o \
./Drivers/Middlewares/TFT_LCD_2.4_320x240/touch_ads.o 

C_DEPS += \
./Drivers/Middlewares/TFT_LCD_2.4_320x240/ILI9341.d \
./Drivers/Middlewares/TFT_LCD_2.4_320x240/fonts.d \
./Drivers/Middlewares/TFT_LCD_2.4_320x240/touch.d \
./Drivers/Middlewares/TFT_LCD_2.4_320x240/touch_ads.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Middlewares/TFT_LCD_2.4_320x240/%.o Drivers/Middlewares/TFT_LCD_2.4_320x240/%.su Drivers/Middlewares/TFT_LCD_2.4_320x240/%.cyclo: ../Drivers/Middlewares/TFT_LCD_2.4_320x240/%.c Drivers/Middlewares/TFT_LCD_2.4_320x240/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"C:/Users/DMXHP/Documents/Programing_Files_IoT/STM32CubeIDE/workspace_2.0.0/STM32F407VET6/TFT_Screen/Drivers/Middlewares/TFT_LCD_2.4_320x240" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Middlewares-2f-TFT_LCD_2-2e-4_320x240

clean-Drivers-2f-Middlewares-2f-TFT_LCD_2-2e-4_320x240:
	-$(RM) ./Drivers/Middlewares/TFT_LCD_2.4_320x240/ILI9341.cyclo ./Drivers/Middlewares/TFT_LCD_2.4_320x240/ILI9341.d ./Drivers/Middlewares/TFT_LCD_2.4_320x240/ILI9341.o ./Drivers/Middlewares/TFT_LCD_2.4_320x240/ILI9341.su ./Drivers/Middlewares/TFT_LCD_2.4_320x240/fonts.cyclo ./Drivers/Middlewares/TFT_LCD_2.4_320x240/fonts.d ./Drivers/Middlewares/TFT_LCD_2.4_320x240/fonts.o ./Drivers/Middlewares/TFT_LCD_2.4_320x240/fonts.su ./Drivers/Middlewares/TFT_LCD_2.4_320x240/touch.cyclo ./Drivers/Middlewares/TFT_LCD_2.4_320x240/touch.d ./Drivers/Middlewares/TFT_LCD_2.4_320x240/touch.o ./Drivers/Middlewares/TFT_LCD_2.4_320x240/touch.su ./Drivers/Middlewares/TFT_LCD_2.4_320x240/touch_ads.cyclo ./Drivers/Middlewares/TFT_LCD_2.4_320x240/touch_ads.d ./Drivers/Middlewares/TFT_LCD_2.4_320x240/touch_ads.o ./Drivers/Middlewares/TFT_LCD_2.4_320x240/touch_ads.su

.PHONY: clean-Drivers-2f-Middlewares-2f-TFT_LCD_2-2e-4_320x240

