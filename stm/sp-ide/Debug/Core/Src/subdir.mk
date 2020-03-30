################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/SP_LCD.c \
../Core/Src/SP_RGB.c \
../Core/Src/SP_SD.c \
../Core/Src/SP_THS.c \
../Core/Src/diskio.c \
../Core/Src/ff.c \
../Core/Src/ffsystem.c \
../Core/Src/ffunicode.c \
../Core/Src/main.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c 

OBJS += \
./Core/Src/SP_LCD.o \
./Core/Src/SP_RGB.o \
./Core/Src/SP_SD.o \
./Core/Src/SP_THS.o \
./Core/Src/diskio.o \
./Core/Src/ff.o \
./Core/Src/ffsystem.o \
./Core/Src/ffunicode.o \
./Core/Src/main.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o 

C_DEPS += \
./Core/Src/SP_LCD.d \
./Core/Src/SP_RGB.d \
./Core/Src/SP_SD.d \
./Core/Src/SP_THS.d \
./Core/Src/diskio.d \
./Core/Src/ff.d \
./Core/Src/ffsystem.d \
./Core/Src/ffunicode.d \
./Core/Src/main.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/SP_LCD.o: ../Core/Src/SP_LCD.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Include -I../Core/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/SP_LCD.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/SP_RGB.o: ../Core/Src/SP_RGB.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Include -I../Core/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/SP_RGB.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/SP_SD.o: ../Core/Src/SP_SD.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Include -I../Core/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/SP_SD.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/SP_THS.o: ../Core/Src/SP_THS.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Include -I../Core/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/SP_THS.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/diskio.o: ../Core/Src/diskio.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Include -I../Core/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/diskio.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/ff.o: ../Core/Src/ff.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Include -I../Core/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/ff.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/ffsystem.o: ../Core/Src/ffsystem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Include -I../Core/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/ffsystem.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/ffunicode.o: ../Core/Src/ffunicode.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Include -I../Core/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/ffunicode.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/main.o: ../Core/Src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Include -I../Core/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/stm32f4xx_hal_msp.o: ../Core/Src/stm32f4xx_hal_msp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Include -I../Core/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/stm32f4xx_hal_msp.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/stm32f4xx_it.o: ../Core/Src/stm32f4xx_it.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Include -I../Core/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/stm32f4xx_it.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/syscalls.o: ../Core/Src/syscalls.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Include -I../Core/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/syscalls.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/sysmem.o: ../Core/Src/sysmem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Include -I../Core/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/sysmem.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/system_stm32f4xx.o: ../Core/Src/system_stm32f4xx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/CMSIS/Include -I../Core/Inc -IC:/Users/bk/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.0/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/system_stm32f4xx.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

