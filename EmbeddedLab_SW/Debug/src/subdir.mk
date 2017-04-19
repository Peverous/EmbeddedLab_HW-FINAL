################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/Interrupts.c \
../src/Timer.c \
../src/UART.c \
../src/crc.c \
../src/main.c \
../src/platform.c 

OBJS += \
./src/Interrupts.o \
./src/Timer.o \
./src/UART.o \
./src/crc.o \
./src/main.o \
./src/platform.o 

C_DEPS += \
./src/Interrupts.d \
./src/Timer.d \
./src/UART.d \
./src/crc.d \
./src/main.d \
./src/platform.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -I../../EmbeddedLab_SW_bsp/microblaze_0/include -mlittle-endian -mcpu=v9.4 -mxl-soft-mul -Wl,--no-relax -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


