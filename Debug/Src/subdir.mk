################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/dma.c \
../Src/gpio.c \
../Src/hts221.c \
../Src/i2c.c \
../Src/lis3mdltr.c \
../Src/lps22hb.c \
../Src/lsm6ds0.c \
../Src/main.c \
../Src/predictw.c \
../Src/spi.c \
../Src/stm32f3xx_it.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32f3xx.c 

OBJS += \
./Src/dma.o \
./Src/gpio.o \
./Src/hts221.o \
./Src/i2c.o \
./Src/lis3mdltr.o \
./Src/lps22hb.o \
./Src/lsm6ds0.o \
./Src/main.o \
./Src/predictw.o \
./Src/spi.o \
./Src/stm32f3xx_it.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32f3xx.o 

C_DEPS += \
./Src/dma.d \
./Src/gpio.d \
./Src/hts221.d \
./Src/i2c.d \
./Src/lis3mdltr.d \
./Src/lps22hb.d \
./Src/lsm6ds0.d \
./Src/main.d \
./Src/predictw.d \
./Src/spi.d \
./Src/stm32f3xx_it.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32f3xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F303x8 -DUSE_FULL_LL_DRIVER -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DEXTERNAL_CLOCK_VALUE=8000000 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../display -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/dma.d ./Src/dma.o ./Src/gpio.d ./Src/gpio.o ./Src/hts221.d ./Src/hts221.o ./Src/i2c.d ./Src/i2c.o ./Src/lis3mdltr.d ./Src/lis3mdltr.o ./Src/lps22hb.d ./Src/lps22hb.o ./Src/lsm6ds0.d ./Src/lsm6ds0.o ./Src/main.d ./Src/main.o ./Src/predictw.d ./Src/predictw.o ./Src/spi.d ./Src/spi.o ./Src/stm32f3xx_it.d ./Src/stm32f3xx_it.o ./Src/syscalls.d ./Src/syscalls.o ./Src/sysmem.d ./Src/sysmem.o ./Src/system_stm32f3xx.d ./Src/system_stm32f3xx.o

.PHONY: clean-Src

