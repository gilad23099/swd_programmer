################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/blink_bin.c \
../Core/Src/main.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_hal_timebase_tim.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/swd_bitbang.c \
../Core/Src/swd_control.c \
../Core/Src/swd_dp_ap.c \
../Core/Src/swd_flash.c \
../Core/Src/swd_gpio.c \
../Core/Src/swd_transfer.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c 

OBJS += \
./Core/Src/blink_bin.o \
./Core/Src/main.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_hal_timebase_tim.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/swd_bitbang.o \
./Core/Src/swd_control.o \
./Core/Src/swd_dp_ap.o \
./Core/Src/swd_flash.o \
./Core/Src/swd_gpio.o \
./Core/Src/swd_transfer.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o 

C_DEPS += \
./Core/Src/blink_bin.d \
./Core/Src/main.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_hal_timebase_tim.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/swd_bitbang.d \
./Core/Src/swd_control.d \
./Core/Src/swd_dp_ap.d \
./Core/Src/swd_flash.d \
./Core/Src/swd_gpio.d \
./Core/Src/swd_transfer.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/blink_bin.cyclo ./Core/Src/blink_bin.d ./Core/Src/blink_bin.o ./Core/Src/blink_bin.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_hal_timebase_tim.cyclo ./Core/Src/stm32f4xx_hal_timebase_tim.d ./Core/Src/stm32f4xx_hal_timebase_tim.o ./Core/Src/stm32f4xx_hal_timebase_tim.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/swd_bitbang.cyclo ./Core/Src/swd_bitbang.d ./Core/Src/swd_bitbang.o ./Core/Src/swd_bitbang.su ./Core/Src/swd_control.cyclo ./Core/Src/swd_control.d ./Core/Src/swd_control.o ./Core/Src/swd_control.su ./Core/Src/swd_dp_ap.cyclo ./Core/Src/swd_dp_ap.d ./Core/Src/swd_dp_ap.o ./Core/Src/swd_dp_ap.su ./Core/Src/swd_flash.cyclo ./Core/Src/swd_flash.d ./Core/Src/swd_flash.o ./Core/Src/swd_flash.su ./Core/Src/swd_gpio.cyclo ./Core/Src/swd_gpio.d ./Core/Src/swd_gpio.o ./Core/Src/swd_gpio.su ./Core/Src/swd_transfer.cyclo ./Core/Src/swd_transfer.d ./Core/Src/swd_transfer.o ./Core/Src/swd_transfer.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su

.PHONY: clean-Core-2f-Src

