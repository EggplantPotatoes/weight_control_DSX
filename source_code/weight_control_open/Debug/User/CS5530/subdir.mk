################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/CS5530/CS5530.c \
../User/CS5530/data_filter.c \
../User/CS5530/soft_SPI.c 

OBJS += \
./User/CS5530/CS5530.o \
./User/CS5530/data_filter.o \
./User/CS5530/soft_SPI.o 

C_DEPS += \
./User/CS5530/CS5530.d \
./User/CS5530/data_filter.d \
./User/CS5530/soft_SPI.d 


# Each subdirectory must supply rules for building sources it contributes
User/CS5530/%.o User/CS5530/%.su User/CS5530/%.cyclo: ../User/CS5530/%.c User/CS5530/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"E:/my_storage_home/develop_case/my_products/weight_control/weight_control_cs5530/weight_control_open/source_code/weight_control_open/User/CS5530" -I"E:/my_storage_home/develop_case/my_products/weight_control/weight_control_cs5530/weight_control_open/source_code/weight_control_open/User/modbus" -I"E:/my_storage_home/develop_case/my_products/weight_control/weight_control_cs5530/weight_control_open/source_code/weight_control_open/User/user_bsp" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-CS5530

clean-User-2f-CS5530:
	-$(RM) ./User/CS5530/CS5530.cyclo ./User/CS5530/CS5530.d ./User/CS5530/CS5530.o ./User/CS5530/CS5530.su ./User/CS5530/data_filter.cyclo ./User/CS5530/data_filter.d ./User/CS5530/data_filter.o ./User/CS5530/data_filter.su ./User/CS5530/soft_SPI.cyclo ./User/CS5530/soft_SPI.d ./User/CS5530/soft_SPI.o ./User/CS5530/soft_SPI.su

.PHONY: clean-User-2f-CS5530

