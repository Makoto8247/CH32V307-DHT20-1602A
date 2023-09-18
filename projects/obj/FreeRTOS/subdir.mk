################################################################################
# MRS Version: 1.9.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/croutine.c \
../FreeRTOS/event_groups.c \
../FreeRTOS/list.c \
../FreeRTOS/queue.c \
../FreeRTOS/stream_buffer.c \
../FreeRTOS/tasks.c \
../FreeRTOS/timers.c 

OBJS += \
./FreeRTOS/croutine.o \
./FreeRTOS/event_groups.o \
./FreeRTOS/list.o \
./FreeRTOS/queue.o \
./FreeRTOS/stream_buffer.o \
./FreeRTOS/tasks.o \
./FreeRTOS/timers.o 

C_DEPS += \
./FreeRTOS/croutine.d \
./FreeRTOS/event_groups.d \
./FreeRTOS/list.d \
./FreeRTOS/queue.d \
./FreeRTOS/stream_buffer.d \
./FreeRTOS/tasks.d \
./FreeRTOS/timers.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/%.o: ../FreeRTOS/%.c
	@	@	riscv-none-embed-gcc -march=rv32imafcxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\MounRiver\MounRiver_Studio\workspace\CH32V307-FreeRTOS\Debug" -I"D:\MounRiver\MounRiver_Studio\workspace\CH32V307-FreeRTOS\Core" -I"D:\MounRiver\MounRiver_Studio\workspace\CH32V307-FreeRTOS\User" -I"D:\MounRiver\MounRiver_Studio\workspace\CH32V307-FreeRTOS\Peripheral\inc" -I"D:\MounRiver\MounRiver_Studio\workspace\CH32V307-FreeRTOS\FreeRTOS" -I"D:\MounRiver\MounRiver_Studio\workspace\CH32V307-FreeRTOS\FreeRTOS\include" -I"D:\MounRiver\MounRiver_Studio\workspace\CH32V307-FreeRTOS\FreeRTOS\portable\Common" -I"D:\MounRiver\MounRiver_Studio\workspace\CH32V307-FreeRTOS\FreeRTOS\portable\GCC\RISC-V" -I"D:\MounRiver\MounRiver_Studio\workspace\CH32V307-FreeRTOS\FreeRTOS\portable\GCC\RISC-V\chip_specific_extensions\RV32I_PFIC_no_extensions" -I"D:\MounRiver\MounRiver_Studio\workspace\CH32V307-FreeRTOS\FreeRTOS\portable\MemMang" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

