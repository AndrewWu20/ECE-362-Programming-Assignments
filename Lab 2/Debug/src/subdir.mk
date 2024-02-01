################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/syscalls.c \
../src/system_stm32f0xx.c 

S_UPPER_SRCS += \
../src/fill_alpha.S \
../src/font.S 

O_SRCS += \
../src/autotest.o 

OBJS += \
./src/fill_alpha.o \
./src/font.o \
./src/main.o \
./src/syscalls.o \
./src/system_stm32f0xx.o 

S_UPPER_DEPS += \
./src/fill_alpha.d \
./src/font.d 

C_DEPS += \
./src/main.d \
./src/syscalls.d \
./src/system_stm32f0xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F0 -DSTM32F091RCTx -DDEBUG -DSTM32F091 -DUSE_STDPERIPH_DRIVER -I"/home/shay/a/wu1795/362/f2022-lab-7-main/StdPeriph_Driver/inc" -I"/home/shay/a/wu1795/362/f2022-lab-7-main/inc" -I"/home/shay/a/wu1795/362/f2022-lab-7-main/CMSIS/device" -I"/home/shay/a/wu1795/362/f2022-lab-7-main/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F0 -DSTM32F091RCTx -DDEBUG -DSTM32F091 -DUSE_STDPERIPH_DRIVER -I"/home/shay/a/wu1795/362/f2022-lab-7-main/StdPeriph_Driver/inc" -I"/home/shay/a/wu1795/362/f2022-lab-7-main/inc" -I"/home/shay/a/wu1795/362/f2022-lab-7-main/CMSIS/device" -I"/home/shay/a/wu1795/362/f2022-lab-7-main/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


