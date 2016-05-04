################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../startup/startup_stm32f10x_md.S 

OBJS += \
./startup/startup_stm32f10x_md.o 

S_UPPER_DEPS += \
./startup/startup_stm32f10x_md.d 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DNUCLEO_F103RB -DSTM32F1 -DSTM32F103RBTx -DSTM32 -DDEBUG -DUSE_STDPERIPH_DRIVER -DSTM32F10X_MD -I/home/marekf/Dokumenty/pwr/mgr/stm32/sonar/inc -I/home/marekf/Dokumenty/pwr/mgr/stm32/sonar/StdPeriph_Driver/inc -I/home/marekf/Dokumenty/pwr/mgr/stm32/sonar/Utilities/STM32F1xx-Nucleo -I/home/marekf/Dokumenty/pwr/mgr/stm32/sonar/CMSIS/device -I/home/marekf/Dokumenty/pwr/mgr/stm32/sonar/CMSIS/core -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


