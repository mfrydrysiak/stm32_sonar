################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/hw_config.c \
../src/main.c \
../src/sonar_ctrl.c \
../src/syscalls.c \
../src/system_stm32f10x.c 

OBJS += \
./src/hw_config.o \
./src/main.o \
./src/sonar_ctrl.o \
./src/syscalls.o \
./src/system_stm32f10x.o 

C_DEPS += \
./src/hw_config.d \
./src/main.d \
./src/sonar_ctrl.d \
./src/syscalls.d \
./src/system_stm32f10x.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DNUCLEO_F103RB -DSTM32F1 -DSTM32F103RBTx -DSTM32 -DDEBUG -DUSE_STDPERIPH_DRIVER -DSTM32F10X_MD -I/home/marekf/Dokumenty/pwr/mgr/stm32/sonar/inc -I/home/marekf/Dokumenty/pwr/mgr/stm32/sonar/StdPeriph_Driver/inc -I/home/marekf/Dokumenty/pwr/mgr/stm32/sonar/Utilities/STM32F1xx-Nucleo -I/home/marekf/Dokumenty/pwr/mgr/stm32/sonar/CMSIS/device -I/home/marekf/Dokumenty/pwr/mgr/stm32/sonar/CMSIS/core -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


