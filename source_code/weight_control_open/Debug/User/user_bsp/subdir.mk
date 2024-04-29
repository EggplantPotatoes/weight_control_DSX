################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/user_bsp/flash.c \
../User/user_bsp/fmc_flash.c \
../User/user_bsp/user_uart.c 

OBJS += \
./User/user_bsp/flash.o \
./User/user_bsp/fmc_flash.o \
./User/user_bsp/user_uart.o 

C_DEPS += \
./User/user_bsp/flash.d \
./User/user_bsp/fmc_flash.d \
./User/user_bsp/user_uart.d 


# Each subdirectory must supply rules for building sources it contributes
User/user_bsp/%.o User/user_bsp/%.su User/user_bsp/%.cyclo: ../User/user_bsp/%.c User/user_bsp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"E:/my_storage_home/develop_case/my_products/weight_control/weight_control_cs5530/weight_control_open/source_code/weight_control_open/User/CS5530" -I"E:/my_storage_home/develop_case/my_products/weight_control/weight_control_cs5530/weight_control_open/source_code/weight_control_open/User/modbus" -I"E:/my_storage_home/develop_case/my_products/weight_control/weight_control_cs5530/weight_control_open/source_code/weight_control_open/User/user_bsp" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-user_bsp

clean-User-2f-user_bsp:
	-$(RM) ./User/user_bsp/flash.cyclo ./User/user_bsp/flash.d ./User/user_bsp/flash.o ./User/user_bsp/flash.su ./User/user_bsp/fmc_flash.cyclo ./User/user_bsp/fmc_flash.d ./User/user_bsp/fmc_flash.o ./User/user_bsp/fmc_flash.su ./User/user_bsp/user_uart.cyclo ./User/user_bsp/user_uart.d ./User/user_bsp/user_uart.o ./User/user_bsp/user_uart.su

.PHONY: clean-User-2f-user_bsp

