################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Audio_DAC/audiodac.c 

OBJS += \
./Drivers/Audio_DAC/audiodac.o 

C_DEPS += \
./Drivers/Audio_DAC/audiodac.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Audio_DAC/%.o Drivers/Audio_DAC/%.su Drivers/Audio_DAC/%.cyclo: ../Drivers/Audio_DAC/%.c Drivers/Audio_DAC/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G491xx -c -I"C:/Users/Implacable/STM32CubeIDE/workspace_1.15.0/Init-Test-2/Drivers/GPIO_XPNDR" -I"C:/Users/Implacable/STM32CubeIDE/workspace_1.15.0/Init-Synth/Drivers/EEPROM" -I"C:/Users/Implacable/STM32CubeIDE/workspace_1.15.0/Init-Test-2/Drivers/Audio_DAC" -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -I../USB_Device/App -I../USB_Device/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Audio_DAC

clean-Drivers-2f-Audio_DAC:
	-$(RM) ./Drivers/Audio_DAC/audiodac.cyclo ./Drivers/Audio_DAC/audiodac.d ./Drivers/Audio_DAC/audiodac.o ./Drivers/Audio_DAC/audiodac.su

.PHONY: clean-Drivers-2f-Audio_DAC
