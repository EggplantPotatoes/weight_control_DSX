################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/modbus/message.c \
../User/modbus/modbus.c 

OBJS += \
./User/modbus/message.o \
./User/modbus/modbus.o 

C_DEPS += \
./User/modbus/message.d \
./User/modbus/modbus.d 


# Each subdirectory must supply rules for building sources it contributes
User/modbus/%.o User/modbus/%.su User/modbus/%.cyclo: ../User/modbus/%.c User/modbus/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"E:/my_storage_home/develop_case/my_products/weight_control/weight_control_cs5530/weight_control_open/source_code/weight_control_open/User/CS5530" -I"E:/my_storage_home/develop_case/my_products/weight_control/weight_control_cs5530/weight_control_open/source_code/weight_control_open/User/modbus" -I"E:/my_storage_home/develop_case/my_products/weight_control/weight_control_cs5530/weight_control_open/source_code/weight_control_open/User/user_bsp" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-modbus

clean-User-2f-modbus:
	-$(RM) ./User/modbus/message.cyclo ./User/modbus/message.d ./User/modbus/message.o ./User/modbus/message.su ./User/modbus/modbus.cyclo ./User/modbus/modbus.d ./User/modbus/modbus.o ./User/modbus/modbus.su

.PHONY: clean-User-2f-modbus

